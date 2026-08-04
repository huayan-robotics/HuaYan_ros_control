#include "elfin_controller_driver/controller_hardware.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <pluginlib/class_list_macros.hpp>

namespace elfin_controller_driver
{
using hardware_interface::CallbackReturn;
using hardware_interface::return_type;

ElfinControllerHardware::~ElfinControllerHardware()
{
  disconnect_clients();
}

std::string ElfinControllerHardware::parameter_string(
  const hardware_interface::HardwareInfo & info, const std::string & key, const std::string & fallback)
{
  const auto it = info.hardware_parameters.find(key);
  return it == info.hardware_parameters.end() ? fallback : it->second;
}
int ElfinControllerHardware::parameter_int(
  const hardware_interface::HardwareInfo & info, const std::string & key, int fallback)
{
  const auto value = parameter_string(info, key, ""); return value.empty() ? fallback : std::stoi(value);
}
double ElfinControllerHardware::parameter_double(
  const hardware_interface::HardwareInfo & info, const std::string & key, double fallback)
{
  const auto value = parameter_string(info, key, ""); return value.empty() ? fallback : std::stod(value);
}

CallbackReturn ElfinControllerHardware::on_init(const hardware_interface::HardwareInfo & info)
{
  if (SystemInterface::on_init(info) != CallbackReturn::SUCCESS) {return CallbackReturn::ERROR;}
  if (info.joints.size() != kJointCount) {
    RCLCPP_ERROR(rclcpp::get_logger("ElfinControllerHardware"), "Expected 6 joints, got %zu", info.joints.size());
    return CallbackReturn::ERROR;
  }
  robot_ip_ = parameter_string(info, "robot_ip", "192.168.1.10");
  state_port_ = parameter_int(info, "state_port", 8893);
  command_port_ = parameter_int(info, "command_port", 8892);
  socket_timeout_ms_ = parameter_int(info, "socket_timeout_ms", 1000);
  state_stale_timeout_ms_ = parameter_int(info, "state_stale_timeout_ms",
    parameter_int(info, "state_timeout_ms", 100));
  state_disconnect_timeout_ms_ = parameter_int(info, "state_disconnect_timeout_ms", 1000);
  if (state_stale_timeout_ms_ <= 0 || state_disconnect_timeout_ms_ <= state_stale_timeout_ms_) {
    RCLCPP_ERROR(rclcpp::get_logger("ElfinControllerHardware"),
      "Require 0 < state_stale_timeout_ms < state_disconnect_timeout_ms");
    return CallbackReturn::ERROR;
  }
  servo_gain_ = parameter_double(info, "servo_gain", 8000.0);
  lookahead_time_ = parameter_double(info, "lookahead_time", 0.004);
  speed_acceleration_deg_ = parameter_double(info, "speed_acceleration_deg", 180.0);
  speed_runtime_ = parameter_double(info, "speed_runtime", 0.02);
  position_command_epsilon_ = parameter_double(info, "position_command_epsilon", 1e-8);
  velocity_command_epsilon_ = parameter_double(info, "velocity_command_epsilon", 1e-8);
  servo_restart_idle_ms_ = parameter_int(info, "servo_restart_idle_ms", 100);
  command_log_throttle_ms_ = parameter_int(info, "command_log_throttle_ms", 100);
  state_publish_rate_ = parameter_double(info, "state_publish_rate", 100.0);
  unit_scale_ = parameter_string(info, "controller_joint_unit", "degree") == "radian" ? 1.0 : M_PI / 180.0;
  positions_.assign(kJointCount, std::nan("")); velocities_.assign(kJointCount, std::nan(""));
  efforts_.assign(kJointCount, std::nan("")); position_commands_.assign(kJointCount, std::nan(""));
  velocity_commands_.assign(kJointCount, 0.0);
  last_position_command_.assign(kJointCount, std::nan(""));
  last_velocity_command_.assign(kJointCount, 0.0);
  state_publisher_node_ = rclcpp::Node::make_shared("elfin_hardware_state_publisher");
  realtime_publisher_ = state_publisher_node_->create_publisher<
    elfin_robot_msgs::msg::ElfinRealtimeState>("/elfin_sdk/realtime_state", 10);
  joint_publisher_ = state_publisher_node_->create_publisher<sensor_msgs::msg::JointState>(
    "/elfin_sdk/joint_states", 10);
  for (const auto & joint : info.joints) {
    if (joint.state_interfaces.size() < 2 || joint.command_interfaces.size() != 2) {
      RCLCPP_ERROR(rclcpp::get_logger("ElfinControllerHardware"),
        "Each joint needs position/velocity state and command interfaces");
      return CallbackReturn::ERROR;
    }
  }
  return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface> ElfinControllerHardware::export_state_interfaces()
{
  std::vector<hardware_interface::StateInterface> result;
  for (std::size_t i = 0; i < kJointCount; ++i) {
    result.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_POSITION, &positions_[i]);
    result.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &velocities_[i]);
    result.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_EFFORT, &efforts_[i]);
  }
  return result;
}
std::vector<hardware_interface::CommandInterface> ElfinControllerHardware::export_command_interfaces()
{
  std::vector<hardware_interface::CommandInterface> result;
  for (std::size_t i = 0; i < kJointCount; ++i) {
    result.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_POSITION, &position_commands_[i]);
    result.emplace_back(info_.joints[i].name, hardware_interface::HW_IF_VELOCITY, &velocity_commands_[i]);
  }
  return result;
}

bool ElfinControllerHardware::connect_clients()
{
  if (!state_client_.connect(
      robot_ip_, state_port_, std::min(socket_timeout_ms_, state_stale_timeout_ms_))) {
    RCLCPP_ERROR(
      rclcpp::get_logger("ElfinControllerHardware"),
      "Cannot connect state channel %s:%d", robot_ip_.c_str(), state_port_);
    return false;
  }
  running_ = true;
  state_thread_ = std::thread(&ElfinControllerHardware::state_loop, this);
  return true;
}
void ElfinControllerHardware::disconnect_clients()
{
  // Wake a blocking recv() first, then let the state thread leave before the
  // descriptor is closed. Closing it before join() caused an EBADF race.
  running_ = false;
  state_client_.interrupt();
  if (state_thread_.joinable()) {state_thread_.join();}
  state_client_.disconnect(); realtime_client_.disconnect();
  state_valid_ = false; have_state_ = false;
}
void ElfinControllerHardware::state_loop()
{
  rclcpp::Clock throttle_clock(RCL_STEADY_TIME);
  auto last_frame = std::chrono::steady_clock::now();
  while (running_) {
    RobotState state; std::string error;
    const auto result = state_client_.receive(state, error);
    if (result != StateClient::ReceiveResult::OK) {
      if (!running_) {break;}
      const auto now = std::chrono::steady_clock::now();
      const auto silent_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_frame).count();
      if (silent_ms >= state_stale_timeout_ms_) {state_valid_ = false;}
      const bool fatal = result == StateClient::ReceiveResult::DISCONNECTED ||
        result == StateClient::ReceiveResult::INVALID_FRAME;
      if (fatal) {
        RCLCPP_ERROR_THROTTLE(
          rclcpp::get_logger("ElfinControllerHardware"), throttle_clock, 2000,
          "8893 state receive failed: %s", error.c_str());
      }
      if (fatal || silent_ms >= state_disconnect_timeout_ms_) {
        if (!fatal) {
          RCLCPP_ERROR_THROTTLE(
            rclcpp::get_logger("ElfinControllerHardware"), throttle_clock, 2000,
            "No complete 8893 state frame for %ld ms; reconnecting", silent_ms);
        }
        state_client_.disconnect();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (!state_client_.connect(
            robot_ip_, state_port_, std::min(socket_timeout_ms_, state_stale_timeout_ms_))) {
          RCLCPP_ERROR_THROTTLE(
            rclcpp::get_logger("ElfinControllerHardware"), throttle_clock, 2000,
            "Cannot reconnect state channel %s:%d", robot_ip_.c_str(), state_port_);
        } else {
          last_frame = std::chrono::steady_clock::now();
        }
      }
      continue;
    }
    {std::lock_guard<std::mutex> lock(state_mutex_); latest_state_ = state;}
    last_frame = state.received_at;
    have_state_ = true;
    if (!state_valid_.exchange(true)) {
      resync_commands_requested_ = true;
      RCLCPP_INFO(rclcpp::get_logger("ElfinControllerHardware"),
        "8893 state stream is active");
    }
  }
}
CallbackReturn ElfinControllerHardware::on_activate(const rclcpp_lifecycle::State &)
{
  if (!connect_clients()) {
    RCLCPP_ERROR(
      rclcpp::get_logger("ElfinControllerHardware"),
      "Cannot activate state connection to %s:%d", robot_ip_.c_str(), state_port_);
    return CallbackReturn::ERROR;
  }
  const auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(2);
  while (!state_valid_ && std::chrono::steady_clock::now() < deadline) {std::this_thread::sleep_for(std::chrono::milliseconds(10));}
  if (!state_valid_) {
    RCLCPP_ERROR(
      rclcpp::get_logger("ElfinControllerHardware"),
      "No valid 8893 state frame received within 2 seconds");
    disconnect_clients();
    return CallbackReturn::ERROR;
  }
  read(rclcpp::Time(0), rclcpp::Duration(0, 0)); sync_commands(); reset_command_baseline();
  owner_ = Owner::CONTROLLER;
  return CallbackReturn::SUCCESS;
}
CallbackReturn ElfinControllerHardware::on_deactivate(const rclcpp_lifecycle::State &)
{
  owner_ = Owner::CONTROLLER; active_mode_ = CommandMode::NONE; disconnect_clients(); return CallbackReturn::SUCCESS;
}
CallbackReturn ElfinControllerHardware::on_cleanup(const rclcpp_lifecycle::State &)
{
  owner_ = Owner::CONTROLLER;
  active_mode_ = CommandMode::NONE;
  disconnect_clients();
  return CallbackReturn::SUCCESS;
}
CallbackReturn ElfinControllerHardware::on_shutdown(const rclcpp_lifecycle::State &)
{
  owner_ = Owner::CONTROLLER;
  active_mode_ = CommandMode::NONE;
  disconnect_clients();
  return CallbackReturn::SUCCESS;
}
CallbackReturn ElfinControllerHardware::on_error(const rclcpp_lifecycle::State &)
{
  owner_ = Owner::CONTROLLER;
  active_mode_ = CommandMode::NONE;
  disconnect_clients();
  return CallbackReturn::SUCCESS;
}
return_type ElfinControllerHardware::read(const rclcpp::Time &, const rclcpp::Duration &)
{
  if (!have_state_) {return return_type::OK;}
  RobotState state;
  {std::lock_guard<std::mutex> lock(state_mutex_); state = latest_state_;}
  if (std::chrono::steady_clock::now() - state.received_at >
      std::chrono::milliseconds(state_stale_timeout_ms_)) {
    state_valid_ = false;
    // A stale state disables write(), but must not make controller_manager
    // tear the hardware lifecycle down. The state thread owns reconnection.
    return return_type::OK;
  }
  for (std::size_t i = 0; i < kJointCount; ++i) {
    positions_[i] = state.actual_position[i] * unit_scale_;
    velocities_[i] = state.actual_velocity[i] * unit_scale_;
    efforts_[i] = state.actual_effort[i];
  }
  publish_realtime_state(state);
  if (owner_ != Owner::ROS || resync_commands_requested_.exchange(false)) {
    sync_commands();
    reset_command_baseline();
  }
  return return_type::OK;
}
void ElfinControllerHardware::publish_realtime_state(const RobotState & state)
{
  const auto steady_now = std::chrono::steady_clock::now();
  if (state_publish_rate_ <= 0.0 ||
      steady_now - last_state_publish_ < std::chrono::duration<double>(1.0 / state_publish_rate_))
  {
    return;
  }
  last_state_publish_ = steady_now;
  constexpr double angle_scale = M_PI / 180.0;
  elfin_robot_msgs::msg::ElfinRealtimeState message;
  message.header.stamp = state_publisher_node_->now();
  message.header.frame_id = "elfin_base_link";
  for (std::size_t i = 0; i < kJointCount; ++i) {
    message.joint_position_target[i] = state.target_position[i] * unit_scale_;
    message.joint_velocity_target[i] = state.target_velocity[i] * unit_scale_;
    message.joint_position_actual[i] = positions_[i];
    message.joint_velocity_actual[i] = velocities_[i];
    message.joint_torque_actual[i] = efforts_[i];
    const double cartesian_scale = i < 3 ? 0.001 : angle_scale;
    message.tcp_position_target[i] = state.target_tcp_position[i] * cartesian_scale;
    message.tcp_velocity_target[i] = state.target_tcp_velocity[i] * cartesian_scale;
    message.tcp_position_actual[i] = state.tcp_position[i] * cartesian_scale;
    message.tcp_velocity_actual[i] = state.tcp_velocity[i] * cartesian_scale;
    message.force_raw[i] = state.force[i];
    message.force_calibrated[i] = state.frame_force[i];
  }
  message.speed_scaling = state.speed_scaling;
  message.controller_time_us = state.controller_time_us;
  message.state_machine = state.state_machine;
  message.force_control_state = state.force_control_state;
  realtime_publisher_->publish(message);

  sensor_msgs::msg::JointState joints;
  joints.header = message.header;
  joints.name = {"elfin_joint1", "elfin_joint2", "elfin_joint3", "elfin_joint4",
    "elfin_joint5", "elfin_joint6"};
  joints.position = positions_; joints.velocity = velocities_; joints.effort = efforts_;
  joint_publisher_->publish(joints);
}
void ElfinControllerHardware::sync_commands()
{
  position_commands_ = positions_; std::fill(velocity_commands_.begin(), velocity_commands_.end(), 0.0);
}
void ElfinControllerHardware::reset_command_baseline()
{
  last_position_command_ = position_commands_;
  last_velocity_command_ = velocity_commands_;
}
bool ElfinControllerHardware::command_changed(
  const std::vector<double> & command, const std::vector<double> & baseline,
  double epsilon) const
{
  for (std::size_t i = 0; i < kJointCount; ++i) {
    if (!std::isfinite(command[i]) || !std::isfinite(baseline[i]) ||
        std::abs(command[i] - baseline[i]) > epsilon) {
      return true;
    }
  }
  return false;
}
return_type ElfinControllerHardware::write(const rclcpp::Time &, const rclcpp::Duration &)
{
  if (owner_ != Owner::ROS || !state_valid_ || active_mode_ == CommandMode::NONE) {return return_type::OK;}
  std::array<double, 6> command{};
  if (active_mode_ == CommandMode::POSITION) {
    // JointTrajectoryController keeps writing its last value every update
    // cycle. Do not enter/refresh ServoJ until the numerical command changes.
    if (!command_changed(position_commands_, last_position_command_, position_command_epsilon_)) {
      if (servo_started_ && last_servo_command_at_ != std::chrono::steady_clock::time_point{} &&
          std::chrono::steady_clock::now() - last_servo_command_at_ >=
          std::chrono::milliseconds(servo_restart_idle_ms_)) {
        servo_started_ = false;
        RCLCPP_INFO(rclcpp::get_logger("ElfinControllerHardware"),
          "ServoJ command stream idle for %d ms; next PushServoJ will start a new Servo session",
          servo_restart_idle_ms_);
      }
      return return_type::OK;
    }
    for (std::size_t i = 0; i < kJointCount; ++i) {command[i] = position_commands_[i] / unit_scale_;}
    if (!servo_started_) {
      RCLCPP_INFO(rclcpp::get_logger("ElfinControllerHardware"),
        "8892 TX StartServo,0,%.6f,%.6f,;", servo_gain_, lookahead_time_);
      servo_started_ = realtime_client_.start_servo(servo_gain_, lookahead_time_);
      RCLCPP_INFO(rclcpp::get_logger("ElfinControllerHardware"),
        "8892 StartServo socket write: %s", servo_started_ ? "success" : "failed");
    }
    if (!servo_started_) {return return_type::ERROR;}
    const auto log_position = [this, &command]() {
      RCLCPP_INFO(rclcpp::get_logger("ElfinControllerHardware"),
        "8892 TX PushServoJ target_deg=[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f] "
        "actual_deg=[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f]",
        command[0], command[1], command[2], command[3], command[4], command[5],
        positions_[0] / unit_scale_, positions_[1] / unit_scale_,
        positions_[2] / unit_scale_, positions_[3] / unit_scale_,
        positions_[4] / unit_scale_, positions_[5] / unit_scale_);
    };
    if (command_log_throttle_ms_ == 0) {
      log_position();
    } else if (command_log_throttle_ms_ > 0) {
      RCLCPP_INFO_THROTTLE(rclcpp::get_logger("ElfinControllerHardware"),
        command_log_clock_, command_log_throttle_ms_,
        "8892 TX PushServoJ target_deg=[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f] "
        "actual_deg=[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f]",
        command[0], command[1], command[2], command[3], command[4], command[5],
        positions_[0] / unit_scale_, positions_[1] / unit_scale_,
        positions_[2] / unit_scale_, positions_[3] / unit_scale_,
        positions_[4] / unit_scale_, positions_[5] / unit_scale_);
    }
    if (!realtime_client_.servo_j(command)) {
      RCLCPP_ERROR(rclcpp::get_logger("ElfinControllerHardware"),
        "8892 PushServoJ socket write failed");
      return return_type::ERROR;
    }
    last_position_command_ = position_commands_;
    last_servo_command_at_ = std::chrono::steady_clock::now();
    return return_type::OK;
  }
  if (!command_changed(velocity_commands_, last_velocity_command_, velocity_command_epsilon_)) {
    return return_type::OK;
  }
  for (std::size_t i = 0; i < kJointCount; ++i) {command[i] = velocity_commands_[i] / unit_scale_;}
  if (command_log_throttle_ms_ == 0) {
    RCLCPP_INFO(rclcpp::get_logger("ElfinControllerHardware"),
      "8892 TX SpeedJ velocity_deg_s=[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f], acceleration=%.6f, runtime=%.6f",
      command[0], command[1], command[2], command[3], command[4], command[5],
      speed_acceleration_deg_, speed_runtime_);
  } else if (command_log_throttle_ms_ > 0) {
    RCLCPP_INFO_THROTTLE(rclcpp::get_logger("ElfinControllerHardware"),
      command_log_clock_, command_log_throttle_ms_,
      "8892 TX SpeedJ velocity_deg_s=[%.6f, %.6f, %.6f, %.6f, %.6f, %.6f], acceleration=%.6f, runtime=%.6f",
      command[0], command[1], command[2], command[3], command[4], command[5],
      speed_acceleration_deg_, speed_runtime_);
  }
  if (!realtime_client_.speed_j(command, speed_acceleration_deg_, speed_runtime_)) {
    RCLCPP_ERROR(rclcpp::get_logger("ElfinControllerHardware"), "8892 SpeedJ socket write failed");
    return return_type::ERROR;
  }
  last_velocity_command_ = velocity_commands_;
  return return_type::OK;
}

bool ElfinControllerHardware::all_interfaces(
  const std::vector<std::string> & interfaces, const std::string & type) const
{
  const auto count = std::count_if(interfaces.begin(), interfaces.end(), [&type](const auto & name) {
    return name.size() > type.size() + 1 && name.compare(name.size() - type.size(), type.size(), type) == 0;
  });
  return count == 0 || count == static_cast<long>(kJointCount);
}
return_type ElfinControllerHardware::prepare_command_mode_switch(
  const std::vector<std::string> & start, const std::vector<std::string> & stop)
{
  if (!all_interfaces(start, hardware_interface::HW_IF_POSITION) ||
      !all_interfaces(start, hardware_interface::HW_IF_VELOCITY) ||
      !all_interfaces(stop, hardware_interface::HW_IF_POSITION) ||
      !all_interfaces(stop, hardware_interface::HW_IF_VELOCITY)) {return return_type::ERROR;}
  const auto has = [](const auto & list, const std::string & type) {
    return std::any_of(list.begin(), list.end(), [&type](const auto & value) {return value.rfind('/' + type) != std::string::npos;});
  };
  if (has(start, hardware_interface::HW_IF_POSITION) && has(start, hardware_interface::HW_IF_VELOCITY)) {return return_type::ERROR;}
  requested_mode_ = has(start, hardware_interface::HW_IF_POSITION) ? CommandMode::POSITION :
    (has(start, hardware_interface::HW_IF_VELOCITY) ? CommandMode::VELOCITY : CommandMode::NONE);
  if (requested_mode_ != CommandMode::NONE && !realtime_client_.connected() &&
      !realtime_client_.connect(robot_ip_, command_port_, socket_timeout_ms_))
  {
    RCLCPP_ERROR(
      rclcpp::get_logger("ElfinControllerHardware"),
      "Cannot prepare ROS command mode: connection to %s:%d failed: %s",
      robot_ip_.c_str(), command_port_, realtime_client_.last_error().c_str());
    requested_mode_ = CommandMode::NONE;
    return return_type::ERROR;
  }
  return return_type::OK;
}
return_type ElfinControllerHardware::perform_command_mode_switch(
  const std::vector<std::string> &, const std::vector<std::string> &)
{
  if (active_mode_ == CommandMode::VELOCITY && requested_mode_ != CommandMode::VELOCITY) {realtime_client_.stop_speed();}
  sync_commands(); reset_command_baseline(); servo_started_ = false;
  last_servo_command_at_ = {}; active_mode_ = requested_mode_;
  // Claiming a complete ROS command interface is the sole transition into ROS ownership.
  owner_ = active_mode_ == CommandMode::NONE ? Owner::CONTROLLER : Owner::ROS;
  if (active_mode_ == CommandMode::NONE) {
    realtime_client_.disconnect();
    RCLCPP_INFO(rclcpp::get_logger("ElfinControllerHardware"),
      "ROS command mode stopped; 8892 connection closed");
  }
  return return_type::OK;
}
}  // namespace elfin_controller_driver

PLUGINLIB_EXPORT_CLASS(
  elfin_controller_driver::ElfinControllerHardware, hardware_interface::SystemInterface)
