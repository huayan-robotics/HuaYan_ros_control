#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <controller_manager_msgs/srv/switch_controller.hpp>
#include <elfin_robot_msgs/msg/elfin_brake_state.hpp>
#include <elfin_robot_msgs/msg/elfin_io_state.hpp>
#include <elfin_robot_msgs/msg/elfin_end_io_state.hpp>
#include <elfin_robot_msgs/msg/elfin_robot_status.hpp>
#include <elfin_robot_msgs/srv/get_int32.hpp>
#include <elfin_robot_msgs/srv/get_payload.hpp>
#include <elfin_robot_msgs/srv/get_pose.hpp>
#include <elfin_robot_msgs/srv/jog.hpp>
#include <elfin_robot_msgs/srv/move_target.hpp>
#include <elfin_robot_msgs/srv/set_analog_io.hpp>
#include <elfin_robot_msgs/srv/set_brake.hpp>
#include <elfin_robot_msgs/srv/set_digital_io.hpp>
#include <elfin_robot_msgs/srv/set_pose.hpp>
#include <elfin_robot_msgs/srv/set_float64.hpp>
#include <elfin_robot_msgs/srv/set_int16.hpp>
#include <elfin_robot_msgs/srv/set_payload.hpp>
#include <elfin_robot_msgs/srv/set_string.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <std_srvs/srv/trigger.hpp>

#include "HR_Pro.h"
#include "elfin_controller_driver/rt_info_client.hpp"

using namespace std::chrono_literals;

class ElfinSdkNode : public rclcpp::Node
{
public:
  ElfinSdkNode() : Node("elfin_sdk")
  {
    robot_ip_ = declare_parameter("robot_ip", "10.20.200.3");
    sdk_port_ = declare_parameter("sdk_port", 10003);
    state_port_ = declare_parameter("state_port", 10004);
    state_socket_timeout_ms_ = declare_parameter("state_socket_timeout_ms", 100);
    state_disconnect_timeout_ms_ = declare_parameter("state_disconnect_timeout_ms", 1000);
    motion_controller_ = declare_parameter("motion_controller", "elfin_arm_controller");
    box_id_ = static_cast<unsigned int>(declare_parameter("box_id", 0));
    robot_id_ = static_cast<unsigned int>(declare_parameter("robot_id", 0));
    io_publish_rate_ = declare_parameter("io_publish_rate", 5.0);
    status_publish_rate_ = declare_parameter("status_publish_rate", 10.0);
    auto_start_ros_control_ = declare_parameter("auto_start_ros_control", false);
    service_group_ = create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);
    client_group_ = create_callback_group(rclcpp::CallbackGroupType::Reentrant);
    switch_client_ = create_client<controller_manager_msgs::srv::SwitchController>(
      "/controller_manager/switch_controller", rmw_qos_profile_services_default, client_group_);
    enable_service_ = create_service<std_srvs::srv::SetBool>("~/set_enabled",
      [this](const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
        std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
        if (!request->data) {startup_activation_pending_ = false;}
        if (!request->data && ros_control_active_) {
          std::lock_guard<std::mutex> lock(mode_mutex_);
          if (!switch_motion_controller(false)) {
            response->success = false;
            response->message = "could not stop ROS motion controller before disabling robot";
            return;
          }
          ros_control_active_ = false;
        }
        const int code = request->data ? HRIF_GrpEnable(box_id_, robot_id_) : HRIF_GrpDisable(box_id_, robot_id_);
        response->success = code == 0;
        response->message = sdk_result(code);
        if (code == 0 && request->data) {
          response->message += "; wait for enabled-ready state, then call set_ros_control(true)";
        }
      }, rmw_qos_profile_services_default, service_group_);
    ros_control_service_ = create_service<std_srvs::srv::SetBool>("~/set_ros_control",
      [this](const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
        std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
        std::lock_guard<std::mutex> lock(mode_mutex_);
        if (!request->data) {startup_activation_pending_ = false;}
        if (request->data && (freedrive_ || force_freedrive_)) {
          response->success = false; response->message = "exit free-drive mode first"; return;
        }
        if (request->data) {
          const std::string not_ready = robot_not_ready_reason();
          if (!not_ready.empty()) {
            response->success = false;
            response->message = "ROS control rejected: " + not_ready;
            return;
          }
        }
        response->success = switch_motion_controller(request->data);
        if (response->success) {
          ros_control_active_ = request->data;
          startup_activation_pending_ = false;
        }
        response->message = response->success ? (request->data ? "ROS control active" : "controller control active") : "controller switch failed";
      }, rmw_qos_profile_services_default, service_group_);
    freedrive_service_ = create_service<std_srvs::srv::SetBool>("~/set_freedrive",
      [this](const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
        std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
        std::lock_guard<std::mutex> lock(mode_mutex_);
        if (request->data && force_freedrive_) {
          response->success = false; response->message = "force freedrive is active"; return;
        }
        if (request->data && ros_control_active_ && !switch_motion_controller(false)) {
          response->success = false; response->message = "could not stop ROS motion controller"; return;
        }
        if (request->data) {ros_control_active_ = false; startup_activation_pending_ = false;}
        const int code = request->data ?
          HRIF_GrpOpenFreeDriver(box_id_, robot_id_) : HRIF_GrpCloseFreeDriver(box_id_, robot_id_);
        if (code == 0) {freedrive_ = request->data;}
        response->success = code == 0; response->message = sdk_result(code);
      }, rmw_qos_profile_services_default, service_group_);
    force_freedrive_service_ = create_service<std_srvs::srv::SetBool>("~/set_force_freedrive",
      [this](const std::shared_ptr<std_srvs::srv::SetBool::Request> request,
        std::shared_ptr<std_srvs::srv::SetBool::Response> response) {
        std::lock_guard<std::mutex> lock(mode_mutex_);
        if (request->data && freedrive_) {
          response->success = false; response->message = "freedrive is active"; return;
        }
        if (request->data && ros_control_active_ && !switch_motion_controller(false)) {
          response->success = false; response->message = "could not stop ROS motion controller"; return;
        }
        if (request->data) {ros_control_active_ = false; startup_activation_pending_ = false;}
        const int code = HRIF_SetForceFreeDriveMode(box_id_, robot_id_, request->data ? 1 : 0);
        if (code == 0) {force_freedrive_ = request->data;}
        response->success = code == 0; response->message = sdk_result(code);
      }, rmw_qos_profile_services_default, service_group_);
    stop_service_ = create_service<std_srvs::srv::Trigger>("~/stop",
      [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
        int code = 0;
        {
          std::lock_guard<std::mutex> lock(sdk_motion_mutex_);
          code = stop_sdk_motion_locked();
        }
        const int estop_code = HRIF_EnterSafetyGuard(box_id_, robot_id_, 1);
        if (estop_code == 0) {soft_estop_active_ = true;}
        response->success = estop_code == 0;
        response->message = estop_code == 0 ? "software emergency stop active" :
          sdk_result(estop_code) + "; motion stop result: " + sdk_result(code);
      }, rmw_qos_profile_services_default, service_group_);
    reset_service_ = create_service<std_srvs::srv::Trigger>("~/reset",
      [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
        int code = 0;
        if (soft_estop_active_) {
          code = HRIF_EnterSafetyGuard(box_id_, robot_id_, 0);
          if (code == 0) {soft_estop_active_ = false;}
        }
        // Safety-guard cancellation is asynchronous on HR6.5.22a. During the
        // short SafeguardHandling/Safeguarding window GrpReset returns 20018.
        // Retry here so one GUI Clear Fault press completes the whole action.
        if (code == 0) {
          for (int attempt = 0; attempt < 15; ++attempt) {
            code = HRIF_GrpReset(box_id_, robot_id_);
            if (code != 20018) {break;}
            std::this_thread::sleep_for(200ms);
          }
        }
        set_response(code, response);
      }, rmw_qos_profile_services_default, service_group_);
    pause_service_ = create_service<std_srvs::srv::Trigger>("~/pause",
      [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
        set_response(HRIF_GrpInterrupt(box_id_, robot_id_), response);
      }, rmw_qos_profile_services_default, service_group_);
    continue_service_ = create_service<std_srvs::srv::Trigger>("~/continue",
      [this](const std::shared_ptr<std_srvs::srv::Trigger::Request>,
        std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
        set_response(HRIF_GrpContinue(box_id_, robot_id_), response);
      }, rmw_qos_profile_services_default, service_group_);
    const auto speed_ratio_callback =
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetFloat64::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetFloat64::Response> response) {
        if (request->data < 0.01 || request->data > 1.0) {
          response->success = false;
          response->message = "speed ratio must be in [0.01, 1.0]";
          return;
        }
        const int code = HRIF_SetOverride(box_id_, robot_id_, request->data);
        set_response(code, response);
        if (code == 0) {
          RCLCPP_INFO(get_logger(), "Speed ratio set to %.3f", request->data);
        }
      };
    // Keep set_override for compatibility and expose the clearer service name
    // requested by applications.
    override_service_ = create_service<elfin_robot_msgs::srv::SetFloat64>(
      "~/set_override", speed_ratio_callback,
      rmw_qos_profile_services_default, service_group_);
    speed_ratio_service_ = create_service<elfin_robot_msgs::srv::SetFloat64>(
      "~/set_speed_ratio", speed_ratio_callback,
      rmw_qos_profile_services_default, service_group_);
    tcp_service_ = create_service<elfin_robot_msgs::srv::SetPose>("~/set_tcp",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetPose::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetPose::Response> response) {
        if (!require_disabled("set TCP", response)) {return;}
        const auto & p = request->pose;
        if (!std::all_of(p.begin(), p.end(), [](double value) {return std::isfinite(value);})) {
          response->success = false; response->message = "TCP values must be finite"; return;
        }
        set_response(HRIF_SetTCP(box_id_, robot_id_, p[0], p[1], p[2], p[3], p[4], p[5]), response);
      }, rmw_qos_profile_services_default, service_group_);
    get_tcp_service_ = create_service<elfin_robot_msgs::srv::GetPose>("~/get_tcp",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::GetPose::Request>,
        std::shared_ptr<elfin_robot_msgs::srv::GetPose::Response> response) {
        auto & p = response->pose;
        const int code = HRIF_ReadCurTCP(
          box_id_, robot_id_, p[0], p[1], p[2], p[3], p[4], p[5]);
        set_response(code, response);
      }, rmw_qos_profile_services_default, service_group_);
    ucs_service_ = create_service<elfin_robot_msgs::srv::SetPose>("~/set_ucs",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetPose::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetPose::Response> response) {
        const auto & p = request->pose;
        set_response(HRIF_SetUCS(box_id_, robot_id_, p[0], p[1], p[2], p[3], p[4], p[5]), response);
      }, rmw_qos_profile_services_default, service_group_);
    tcp_name_service_ = create_service<elfin_robot_msgs::srv::SetString>("~/set_tcp_by_name",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetString::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetString::Response> response) {
        if (!require_disabled("set TCP by name", response)) {return;}
        set_response(HRIF_SetTCPByName(box_id_, robot_id_, request->data), response);
      }, rmw_qos_profile_services_default, service_group_);
    ucs_name_service_ = create_service<elfin_robot_msgs::srv::SetString>("~/set_ucs_by_name",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetString::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetString::Response> response) {
        set_response(HRIF_SetUCSByName(box_id_, robot_id_, request->data), response);
      }, rmw_qos_profile_services_default, service_group_);
    digital_io_service_ = create_service<elfin_robot_msgs::srv::SetDigitalIO>("~/set_digital_io",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetDigitalIO::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetDigitalIO::Response> response) {
        int code = -1;
        if (request->domain == "box_do") {code = HRIF_SetBoxDO(box_id_, request->index, request->value);}
        else if (request->domain == "box_co") {code = HRIF_SetBoxCO(box_id_, request->index, request->value);}
        else if (request->domain == "end_do") {code = HRIF_SetEndDO(box_id_, robot_id_, request->index, request->value);}
        if (code < 0) {response->success = false; response->message = "domain must be box_do, box_co, or end_do";}
        else {set_response(code, response);}
      }, rmw_qos_profile_services_default, service_group_);
    analog_io_service_ = create_service<elfin_robot_msgs::srv::SetAnalogIO>("~/set_analog_io",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetAnalogIO::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetAnalogIO::Response> response) {
        set_response(HRIF_SetBoxAOVal(box_id_, request->index, request->value, request->mode), response);
      }, rmw_qos_profile_services_default, service_group_);
    collision_level_service_ = create_service<elfin_robot_msgs::srv::SetInt16>(
      "~/set_collision_level",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetInt16::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetInt16::Response> response) {
        if (!require_disabled("set collision level", response)) {return;}
        if (request->data < 0 || request->data > 5) {
          response->success = false;
          response->message = "collision level must be in [0, 5]";
          return;
        }
        set_response(HRIF_SetCollideLevel(box_id_, robot_id_, request->data), response);
      }, rmw_qos_profile_services_default, service_group_);
    get_collision_level_service_ = create_service<elfin_robot_msgs::srv::GetInt32>(
      "~/get_collision_level",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::GetInt32::Request>,
        std::shared_ptr<elfin_robot_msgs::srv::GetInt32::Response> response) {
        int level = 0;
        const int code = HRIF_GetCollideLevel(box_id_, robot_id_, level);
        response->data = level;
        set_response(code, response);
      }, rmw_qos_profile_services_default, service_group_);
    payload_service_ = create_service<elfin_robot_msgs::srv::SetPayload>("~/set_payload",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetPayload::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetPayload::Response> response) {
        if (!require_disabled("set payload", response)) {return;}
        const auto & cog = request->center_of_gravity;
        if (!std::isfinite(request->mass) ||
          !std::all_of(cog.begin(), cog.end(), [](double value) {return std::isfinite(value);}))
        {
          response->success = false; response->message = "payload values must be finite"; return;
        }
        if (request->mass < 0.0) {
          response->success = false; response->message = "payload mass must be non-negative"; return;
        }
        if (request->option != 1 && request->option != 3) {
          response->success = false; response->message = "payload option must be 1 or 3"; return;
        }
        double max_payload = 0.0;
        const int max_code = HRIF_ReadMaxPayload(box_id_, robot_id_, max_payload);
        if (max_code != 0) {set_response(max_code, response); return;}
        if (request->mass > max_payload) {
          response->success = false;
          response->message = "payload mass exceeds controller maximum " +
            std::to_string(max_payload) + " kg";
          return;
        }
        set_response(HRIF_SetPayload(
          box_id_, robot_id_, request->mass, cog[0], cog[1], cog[2], request->option), response);
      }, rmw_qos_profile_services_default, service_group_);
    get_payload_service_ = create_service<elfin_robot_msgs::srv::GetPayload>("~/get_payload",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::GetPayload::Request>,
        std::shared_ptr<elfin_robot_msgs::srv::GetPayload::Response> response) {
        auto & cog = response->center_of_gravity;
        int code = HRIF_ReadPayload(
          box_id_, robot_id_, response->mass, cog[0], cog[1], cog[2]);
        if (code == 0) {
          code = HRIF_ReadMaxPayload(box_id_, robot_id_, response->max_payload);
        }
        set_response(code, response);
      }, rmw_qos_profile_services_default, service_group_);
    brake_service_ = create_service<elfin_robot_msgs::srv::SetBrake>("~/set_brake",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::SetBrake::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::SetBrake::Response> response) {
        if (!require_disabled("change brake state", response)) {return;}
        if (request->axis > 5) {
          response->success = false; response->message = "brake axis must be in [0, 5]"; return;
        }
        const int code = request->release ?
          HRIF_OpenBrake(box_id_, robot_id_, request->axis) :
          HRIF_CloseBrake(box_id_, robot_id_, request->axis);
        set_response(code, response);
      }, rmw_qos_profile_services_default, service_group_);
    jog_service_ = create_service<elfin_robot_msgs::srv::Jog>("~/jog",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::Jog::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::Jog::Response> response) {
        handle_jog(request, response);
      }, rmw_qos_profile_services_default, service_group_);
    move_target_service_ = create_service<elfin_robot_msgs::srv::MoveTarget>("~/move_target",
      [this](const std::shared_ptr<elfin_robot_msgs::srv::MoveTarget::Request> request,
        std::shared_ptr<elfin_robot_msgs::srv::MoveTarget::Response> response) {
        handle_move_target(request, response);
      }, rmw_qos_profile_services_default, service_group_);
    status_publisher_ = create_publisher<elfin_robot_msgs::msg::ElfinRobotStatus>("~/robot_status", 10);
    brake_publisher_ = create_publisher<elfin_robot_msgs::msg::ElfinBrakeState>(
      "~/brake_state", 10);
    io_publisher_ = create_publisher<elfin_robot_msgs::msg::ElfinIOState>("~/io_state", 10);
    end_io_publisher_ = create_publisher<elfin_robot_msgs::msg::ElfinEndIOState>(
      "~/end_io_state", 10);
    startup_activation_timer_ = create_wall_timer(
      1000ms, std::bind(&ElfinSdkNode::try_startup_ros_control, this), client_group_);
    motion_watchdog_timer_ = create_wall_timer(
      50ms, std::bind(&ElfinSdkNode::check_motion_watchdog, this), service_group_);
  }

  ~ElfinSdkNode() override
  {
    stop_sdk_motion("SDK node shutdown");
    state_running_ = false;
    state_client_.interrupt();
    if (state_thread_.joinable()) {state_thread_.join();}
    state_client_.disconnect();
    if (sdk_connected_) {HRIF_DisConnect(box_id_); sdk_connected_ = false;}
  }

  bool connect_sdk()
  {
    const int code = HRIF_Connect(box_id_, robot_ip_.c_str(), static_cast<unsigned short>(sdk_port_));
    if (code != 0) {RCLCPP_ERROR(get_logger(), "SDK connection failed: %s", sdk_result(code).c_str()); return false;}
    sdk_connected_ = true;
    RCLCPP_INFO(get_logger(), "SDK connected to %s:%d", robot_ip_.c_str(), sdk_port_);
    state_running_ = true;
    state_thread_ = std::thread(&ElfinSdkNode::state_loop, this);
    return true;
  }

private:
  template<typename ResponseT>
  bool require_sdk_motion_ready(
    const std::string & operation, const std::shared_ptr<ResponseT> & response) const
  {
    const std::string reason = robot_not_ready_reason();
    if (!reason.empty()) {
      response->success = false; response->message = operation + " rejected: " + reason; return false;
    }
    if (ros_control_active_) {
      response->success = false; response->message = operation + " rejected: ROS control is active"; return false;
    }
    if (freedrive_ || force_freedrive_) {
      response->success = false; response->message = operation + " rejected: free drive is active"; return false;
    }
    return true;
  }
  void handle_jog(
    const std::shared_ptr<elfin_robot_msgs::srv::Jog::Request> & request,
    const std::shared_ptr<elfin_robot_msgs::srv::Jog::Response> & response)
  {
    using Jog = elfin_robot_msgs::srv::Jog::Request;
    if (request->mode > Jog::MODE_CARTESIAN || request->axis > 5 ||
      request->direction > Jog::DIRECTION_POSITIVE || request->action > Jog::ACTION_KEEPALIVE)
    {response->success = false; response->message = "invalid Jog request"; return;}
    std::lock_guard<std::mutex> lock(sdk_motion_mutex_);
    if (request->action == Jog::ACTION_STOP) {
      if (!sdk_motion_active_) {
        response->success = true; response->message = "already stopped"; return;
      }
      if (sdk_motion_kind_ != 1 || request->mode != jog_mode_ ||
        request->axis != jog_axis_ || request->direction != jog_direction_)
      {
        response->success = false;
        response->message = "Jog stop does not match active Jog";
        return;
      }
      set_response(stop_sdk_motion_locked(), response); return;
    }
    if (!require_sdk_motion_ready("Jog", response)) {return;}
    if (request->action == Jog::ACTION_KEEPALIVE) {
      if (!sdk_motion_active_ || sdk_motion_kind_ != 1 || request->mode != jog_mode_ ||
        request->axis != jog_axis_ || request->direction != jog_direction_)
      {response->success = false; response->message = "Jog keepalive does not match active Jog"; return;}
      const int code = HRIF_LongMoveEvent(box_id_, robot_id_);
      if (code == 0) {motion_deadline_ = std::chrono::steady_clock::now() + 500ms;}
      set_response(code, response); return;
    }
    if (sdk_motion_active_) {stop_sdk_motion_locked();}
    const int code = request->mode == Jog::MODE_JOINT ?
      HRIF_LongJogJ(box_id_, robot_id_, request->axis, request->direction, 1) :
      HRIF_LongJogL(box_id_, robot_id_, request->axis, request->direction, 1);
    if (code == 0) {
      sdk_motion_active_ = true; sdk_motion_kind_ = 1; jog_mode_ = request->mode;
      jog_axis_ = request->axis; jog_direction_ = request->direction;
      motion_deadline_ = std::chrono::steady_clock::now() + 500ms;
    }
    set_response(code, response);
  }
  void handle_move_target(
    const std::shared_ptr<elfin_robot_msgs::srv::MoveTarget::Request> & request,
    const std::shared_ptr<elfin_robot_msgs::srv::MoveTarget::Response> & response)
  {
    using Move = elfin_robot_msgs::srv::MoveTarget::Request;
    if (request->mode > Move::MODE_ALIGN_Z || request->action > Move::ACTION_KEEPALIVE) {
      response->success = false; response->message = "invalid MoveTarget request"; return;
    }
    std::lock_guard<std::mutex> lock(sdk_motion_mutex_);
    if (request->action == Move::ACTION_STOP) {
      if (!sdk_motion_active_) {
        response->success = true; response->message = "already stopped"; return;
      }
      if (sdk_motion_kind_ != 2) {
        response->success = false;
        response->message = "MoveTarget stop does not match active motion";
        return;
      }
      set_response(stop_sdk_motion_locked(), response); return;
    }
    if (!require_sdk_motion_ready("MoveTarget", response)) {return;}
    if (request->action == Move::ACTION_KEEPALIVE) {
      if (!sdk_motion_active_ || sdk_motion_kind_ != 2 || !move_hold_required_) {
        response->success = false; response->message = "no hold-to-run target motion is active"; return;
      }
      motion_deadline_ = std::chrono::steady_clock::now() + 500ms;
      response->success = true; response->message = "success"; return;
    }
    if (!std::all_of(request->target.begin(), request->target.end(),
      [](double value) {return std::isfinite(value);}) || request->velocity <= 0.0 ||
      request->acceleration <= 0.0 || request->blend_radius < 0.0)
    {response->success = false; response->message = "invalid MoveTarget numeric value"; return;}
    const bool align_z = request->mode == Move::MODE_ALIGN_Z;
    const bool joint = request->mode != Move::MODE_CARTESIAN;
    auto target = request->target;
    if (align_z) {
      bool reached = false;
      const int align_code = HRIF_MoveAlignToZ(
        box_id_, robot_id_, "TCP", "Base", reached, target[0], target[1], target[2],
        target[3], target[4], target[5]);
      if (align_code != 0) {set_response(align_code, response); return;}
      if (reached) {
        response->success = true; response->message = "TCP Z-axis is already aligned"; return;
      }
    }
    const auto & t = target;
    std::array<double, 6> reference_joints{};
    if (!joint) {
      double x = 0.0, y = 0.0, z = 0.0, rx = 0.0, ry = 0.0, rz = 0.0;
      double tcp_x = 0.0, tcp_y = 0.0, tcp_z = 0.0;
      double tcp_rx = 0.0, tcp_ry = 0.0, tcp_rz = 0.0;
      double ucs_x = 0.0, ucs_y = 0.0, ucs_z = 0.0;
      double ucs_rx = 0.0, ucs_ry = 0.0, ucs_rz = 0.0;
      const int read_code = HRIF_ReadActPos(
        box_id_, robot_id_, x, y, z, rx, ry, rz,
        reference_joints[0], reference_joints[1], reference_joints[2],
        reference_joints[3], reference_joints[4], reference_joints[5],
        tcp_x, tcp_y, tcp_z, tcp_rx, tcp_ry, tcp_rz,
        ucs_x, ucs_y, ucs_z, ucs_rx, ucs_ry, ucs_rz);
      if (read_code != 0) {set_response(read_code, response); return;}
    }
    move_hold_required_ = false;
    const int code = HRIF_WayPoint(box_id_, robot_id_, joint ? 0 : 1,
      joint ? 0.0 : t[0], joint ? 0.0 : t[1], joint ? 0.0 : t[2],
      joint ? 0.0 : t[3], joint ? 0.0 : t[4], joint ? 0.0 : t[5],
      joint ? t[0] : reference_joints[0], joint ? t[1] : reference_joints[1],
      joint ? t[2] : reference_joints[2], joint ? t[3] : reference_joints[3],
      joint ? t[4] : reference_joints[4], joint ? t[5] : reference_joints[5],
      "TCP", "Base", request->velocity, request->acceleration, request->blend_radius,
      joint ? 1 : 0, 0, 0, 0, align_z ? "ros_gui_align_z" : "ros_gui_target");
    if (code == 0 && request->hold_required) {
      sdk_motion_active_ = true; sdk_motion_kind_ = 2; move_hold_required_ = true;
      motion_deadline_ = std::chrono::steady_clock::now() + 500ms;
    }
    set_response(code, response);
  }
  void check_motion_watchdog()
  {
    std::lock_guard<std::mutex> lock(sdk_motion_mutex_);
    if (sdk_motion_active_ && std::chrono::steady_clock::now() > motion_deadline_) {
      stop_sdk_motion_locked();
      RCLCPP_ERROR(get_logger(), "SDK GUI motion watchdog expired; robot stop requested");
    }
  }
  int stop_sdk_motion_locked()
  {
    const int stopped_kind = sdk_motion_kind_;
    int jog_code = 0;
    if (sdk_motion_active_ && sdk_motion_kind_ == 1) {
      jog_code = jog_mode_ == elfin_robot_msgs::srv::Jog::Request::MODE_JOINT ?
        HRIF_LongJogJ(box_id_, robot_id_, jog_axis_, jog_direction_, 0) :
        HRIF_LongJogL(box_id_, robot_id_, jog_axis_, jog_direction_, 0);
    }
    const int stop_code = HRIF_GrpStop(box_id_, robot_id_);
    int standby_code = 0;
    if (stop_code == 0 && (stopped_kind == 1 || stopped_kind == 2)) {
      // Both LongJog and WayPoint Hold-to-run can remain in the controller's
      // long-motion FSM after their stop call on HR6.5.22a. Launch the SDK's
      // documented Standby recovery while this same SDK connection remains
      // alive so the next GUI command is accepted.
      standby_code = HRIF_XToStandby(box_id_, robot_id_);
    }
    sdk_motion_active_ = false;
    sdk_motion_kind_ = 0;
    move_hold_required_ = false;
    if (jog_code != 0) {return jog_code;}
    if (stop_code != 0) {return stop_code;}
    return standby_code;
  }
  void stop_sdk_motion(const char * reason)
  {
    std::lock_guard<std::mutex> lock(sdk_motion_mutex_);
    if (!sdk_motion_active_ || !sdk_connected_) {return;}
    const int code = stop_sdk_motion_locked();
    RCLCPP_WARN(get_logger(), "%s: SDK motion stop returned %s", reason, sdk_result(code).c_str());
  }
  template<typename ResponseT>
  void set_response(int code, const std::shared_ptr<ResponseT> & response)
  {
    response->success = code == 0; response->message = sdk_result(code);
  }
  template<typename ResponseT>
  bool require_disabled(
    const std::string & operation, const std::shared_ptr<ResponseT> & response) const
  {
    if (!state_received_) {
      response->success = false;
      response->message = operation + " rejected: no 10004 robot status has been received";
      return false;
    }
    if (robot_enabled_) {
      response->success = false;
      response->message = operation + " rejected: robot must be disabled";
      return false;
    }
    return true;
  }
  void publish_robot_status(const elfin_controller_driver::RtInfo & info)
  {
    elfin_robot_msgs::msg::ElfinRobotStatus message;
    message.header.stamp = now();
    message.sdk_connected = sdk_connected_;
    message.moving = info.moving != 0;
    message.enabled = info.enabled != 0;
    message.error_code = info.error_code != 0 ? info.error_code : info.axis_group_error;
    message.error_axis = info.error_axis;
    message.error = message.error_code != 0 ||
      std::any_of(info.axis_error.begin(), info.axis_error.end(), [](int value) {return value != 0;});
    // BrakeState is the per-axis manual brake-open flag, not the controller FSM.
    // In normal enabled operation the controller releases the brakes while this flag remains zero.
    const bool all_manually_released = std::all_of(
      info.brake_state.begin(), info.brake_state.begin() + 6, [](int value) {return value != 0;});
    message.braking = !message.enabled && !all_manually_released;
    message.paused = info.paused != 0;
    // Physical E-stop fields are not present in 10004. This flag reports the
    // software safety-guard E-stop requested through this SDK node.
    message.emergency_stop = soft_estop_active_;
    message.safeguard_stop = soft_estop_active_;
    message.electrified = false;
    message.controller_connected = true;
    message.blending_done = info.blending_done != 0;
    message.in_position = info.in_position != 0;
    {
      std::lock_guard<std::mutex> lock(mode_mutex_);
      message.freedrive = info.freedrive_mode != 0;
      message.force_freedrive = force_freedrive_;
      message.ros_control_active = ros_control_active_;
    }
    status_publisher_->publish(message);

    elfin_robot_msgs::msg::ElfinBrakeState brake_message;
    brake_message.header = message.header;
    for (std::size_t i = 0; i < 6; ++i) {
      brake_message.raw_state[i] = info.brake_state[i];
      // Verified on HR6.5.22a virtual controller: OpenBrake changes the selected
      // axis from 0 to 1. Enabled operation also releases the physical brakes
      // without setting this manual-open flag.
      brake_message.released[i] = message.enabled || info.brake_state[i] != 0;
    }
    brake_publisher_->publish(brake_message);
  }
  void update_robot_readiness(const elfin_controller_driver::RtInfo & info)
  {
    const bool enabled = info.enabled != 0;
    robot_moving_ = info.moving != 0;
    const bool error = info.error_code != 0 || info.axis_group_error != 0 ||
      std::any_of(info.axis_error.begin(), info.axis_error.end(), [](int value) {return value != 0;});
    const bool all_manually_released = std::all_of(
      info.brake_state.begin(), info.brake_state.begin() + 6, [](int value) {return value != 0;});
    const bool braking = !enabled && !all_manually_released;
    robot_enabled_ = enabled;
    robot_error_ = error;
    robot_paused_ = info.paused != 0;
    robot_braking_ = braking;
    state_received_ = true;
    {
      std::lock_guard<std::mutex> lock(mode_mutex_);
      freedrive_ = info.freedrive_mode != 0;
    }

    const bool ready = enabled && !error && !robot_paused_ && !braking;
    if (!startup_state_decided_.exchange(true)) {
      if (auto_start_ros_control_ && ready) {
        startup_activation_pending_ = true;
        RCLCPP_INFO(get_logger(),
          "Robot was enabled-ready at startup; ROS motion controller will be activated automatically");
      } else if (auto_start_ros_control_) {
        RCLCPP_WARN(get_logger(),
          "Robot was not enabled-ready at startup; ROS control remains inactive and must be enabled explicitly with set_ros_control(true) after recovery");
      }
    }

    if (!ready) {startup_activation_pending_ = false;}
    if (ros_control_active_ && !ready) {
      std::lock_guard<std::mutex> lock(mode_mutex_);
      if (ros_control_active_ && switch_motion_controller(false)) {
        ros_control_active_ = false;
        RCLCPP_WARN(get_logger(),
          "ROS motion controller deactivated because robot left enabled-ready state");
      } else if (ros_control_active_) {
        RCLCPP_ERROR(get_logger(),
          "Robot left enabled-ready state but ROS motion controller deactivation failed");
      }
    }
  }
  std::string robot_not_ready_reason() const
  {
    if (!state_received_) {return "no 10004 robot status has been received";}
    if (!robot_enabled_) {return "robot is not enabled";}
    if (robot_error_) {return "robot has an active fault";}
    if (robot_paused_) {return "robot is paused";}
    if (robot_braking_) {return "one or more joint brakes are engaged";}
    return {};
  }
  void try_startup_ros_control()
  {
    if (!startup_activation_pending_ || ros_control_active_) {return;}
    if (!robot_not_ready_reason().empty()) {
      startup_activation_pending_ = false;
      return;
    }
    std::lock_guard<std::mutex> lock(mode_mutex_);
    if (!startup_activation_pending_ || ros_control_active_) {return;}
    if (switch_motion_controller(true)) {
      ros_control_active_ = true;
      startup_activation_pending_ = false;
      RCLCPP_INFO(get_logger(), "ROS motion controller activated automatically");
    } else {
      RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 2000,
        "Waiting for motion controller '%s' to become available for automatic activation",
        motion_controller_.c_str());
    }
  }
  void publish_io_state(const elfin_controller_driver::RtInfo & info)
  {
    elfin_robot_msgs::msg::ElfinIOState message; message.header.stamp = now();
    message.digital_inputs.assign(info.box_di.begin(), info.box_di.end());
    message.digital_outputs.assign(info.box_do.begin(), info.box_do.end());
    message.configurable_inputs.assign(info.box_ci.begin(), info.box_ci.end());
    message.configurable_outputs.assign(info.box_co.begin(), info.box_co.end());
    message.analog_inputs.assign(info.analog_input.begin(), info.analog_input.end());
    message.analog_output_modes.assign(
      info.analog_output_mode.begin(), info.analog_output_mode.end());
    message.analog_outputs.assign(info.analog_output.begin(), info.analog_output.end());
    io_publisher_->publish(message);

    elfin_robot_msgs::msg::ElfinEndIOState end_message;
    end_message.header = message.header;
    end_message.digital_inputs.assign(info.end_di.begin(), info.end_di.end());
    end_message.digital_outputs.assign(info.end_do.begin(), info.end_do.end());
    end_message.buttons.assign(info.end_buttons.begin(), info.end_buttons.end());
    end_message.buttons_enabled = info.end_buttons_enabled != 0;
    end_message.analog_inputs.assign(info.end_ai.begin(), info.end_ai.end());
    end_io_publisher_->publish(end_message);
  }
  void state_loop()
  {
    rclcpp::Clock throttle_clock(RCL_STEADY_TIME);
    auto last_frame = std::chrono::steady_clock::now();
    while (state_running_) {
      if (!state_client_.connect(robot_ip_, state_port_, state_socket_timeout_ms_)) {
        RCLCPP_ERROR_THROTTLE(get_logger(), throttle_clock, 2000,
          "Cannot connect pushed-state channel %s:%d", robot_ip_.c_str(), state_port_);
        std::this_thread::sleep_for(500ms);
        continue;
      }
      RCLCPP_INFO(get_logger(), "10004 pushed-state channel connected to %s:%d",
        robot_ip_.c_str(), state_port_);
      last_frame = std::chrono::steady_clock::now();
      bool frame_layout_reported = false;
      while (state_running_) {
        elfin_controller_driver::RtInfo info;
        std::string error;
        const auto result = state_client_.receive(info, error);
        const auto steady_now = std::chrono::steady_clock::now();
        if (result == elfin_controller_driver::RtInfoClient::ReceiveResult::OK) {
          if (!frame_layout_reported) {
            RCLCPP_INFO(get_logger(),
              "10004 JSON status stream active: total_size=%u, data_size=%u",
              info.frame_size, info.payload_size);
            frame_layout_reported = true;
          }
          last_frame = steady_now;
          update_robot_readiness(info);
          if (status_publish_rate_ > 0.0 &&
              steady_now - last_status_publish_ >=
              std::chrono::duration<double>(1.0 / status_publish_rate_)) {
            last_status_publish_ = steady_now; publish_robot_status(info);
          }
          if (io_publish_rate_ > 0.0 &&
              steady_now - last_io_publish_ >=
              std::chrono::duration<double>(1.0 / io_publish_rate_)) {
            last_io_publish_ = steady_now; publish_io_state(info);
          }
          continue;
        }
        const auto silent_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
          steady_now - last_frame).count();
        if (result == elfin_controller_driver::RtInfoClient::ReceiveResult::DISCONNECTED ||
            silent_ms >= state_disconnect_timeout_ms_) {
          state_received_ = false;
          startup_activation_pending_ = false;
          stop_sdk_motion("10004 status stream lost");
          RCLCPP_ERROR_THROTTLE(get_logger(), throttle_clock, 2000,
            "10004 pushed-state stream disconnected: %s", error.empty() ? "timeout" : error.c_str());
          if (ros_control_active_) {
            std::lock_guard<std::mutex> lock(mode_mutex_);
            if (ros_control_active_ && switch_motion_controller(false)) {
              ros_control_active_ = false;
              RCLCPP_WARN(get_logger(),
                "ROS motion controller deactivated because the 10004 status stream was lost");
            }
          }
          state_client_.disconnect();
          break;
        }
      }
    }
  }
  std::string sdk_result(int code)
  {
    if (code == 0) {return "success";}
    std::string text;
    if (HRIF_IsConnected(box_id_)) {HRIF_GetErrorCodeStr(box_id_, code, text);}
    return "SDK error " + std::to_string(code) + (text.empty() ? "" : ": " + text);
  }
  bool switch_motion_controller(bool start)
  {
    if (!switch_client_->wait_for_service(2s)) {return false;}
    auto request = std::make_shared<controller_manager_msgs::srv::SwitchController::Request>();
    if (start) {request->activate_controllers.push_back(motion_controller_);}
    else {request->deactivate_controllers.push_back(motion_controller_);}
    request->strictness = start ? controller_manager_msgs::srv::SwitchController::Request::STRICT :
      controller_manager_msgs::srv::SwitchController::Request::BEST_EFFORT;
    request->activate_asap = true;
    request->timeout.sec = 2;
    auto future = switch_client_->async_send_request(request);
    return future.wait_for(3s) == std::future_status::ready && future.get()->ok;
  }

  std::string robot_ip_, motion_controller_;
  int sdk_port_, state_port_, state_socket_timeout_ms_, state_disconnect_timeout_ms_;
  double io_publish_rate_, status_publish_rate_;
  bool auto_start_ros_control_{false};
  unsigned int box_id_, robot_id_;
  bool freedrive_{false};
  bool force_freedrive_{false};
  std::mutex mode_mutex_;
  rclcpp::CallbackGroup::SharedPtr service_group_, client_group_;
  rclcpp::Client<controller_manager_msgs::srv::SwitchController>::SharedPtr switch_client_;
  rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr enable_service_, ros_control_service_,
    freedrive_service_, force_freedrive_service_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr stop_service_;
  rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr reset_service_, pause_service_,
    continue_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetFloat64>::SharedPtr override_service_, speed_ratio_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetPose>::SharedPtr tcp_service_, ucs_service_;
  rclcpp::Service<elfin_robot_msgs::srv::GetPose>::SharedPtr get_tcp_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetString>::SharedPtr tcp_name_service_, ucs_name_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetDigitalIO>::SharedPtr digital_io_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetAnalogIO>::SharedPtr analog_io_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetInt16>::SharedPtr collision_level_service_;
  rclcpp::Service<elfin_robot_msgs::srv::GetInt32>::SharedPtr get_collision_level_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetPayload>::SharedPtr payload_service_;
  rclcpp::Service<elfin_robot_msgs::srv::GetPayload>::SharedPtr get_payload_service_;
  rclcpp::Service<elfin_robot_msgs::srv::SetBrake>::SharedPtr brake_service_;
  rclcpp::Service<elfin_robot_msgs::srv::Jog>::SharedPtr jog_service_;
  rclcpp::Service<elfin_robot_msgs::srv::MoveTarget>::SharedPtr move_target_service_;
  rclcpp::Publisher<elfin_robot_msgs::msg::ElfinRobotStatus>::SharedPtr status_publisher_;
  rclcpp::Publisher<elfin_robot_msgs::msg::ElfinBrakeState>::SharedPtr brake_publisher_;
  rclcpp::Publisher<elfin_robot_msgs::msg::ElfinIOState>::SharedPtr io_publisher_;
  rclcpp::Publisher<elfin_robot_msgs::msg::ElfinEndIOState>::SharedPtr end_io_publisher_;
  elfin_controller_driver::RtInfoClient state_client_;
  std::thread state_thread_;
  std::atomic<bool> state_running_{false};
  std::atomic<bool> sdk_connected_{false};
  std::atomic<bool> state_received_{false};
  std::atomic<bool> robot_enabled_{false};
  std::atomic<bool> robot_moving_{false};
  std::atomic<bool> soft_estop_active_{false};
  std::atomic<bool> robot_error_{false};
  std::atomic<bool> robot_paused_{false};
  std::atomic<bool> robot_braking_{true};
  std::atomic<bool> ros_control_active_{false};
  std::atomic<bool> startup_state_decided_{false};
  std::atomic<bool> startup_activation_pending_{false};
  rclcpp::TimerBase::SharedPtr startup_activation_timer_, motion_watchdog_timer_;
  std::mutex sdk_motion_mutex_;
  bool sdk_motion_active_{false}, move_hold_required_{false};
  int sdk_motion_kind_{0}, jog_mode_{0}, jog_axis_{0}, jog_direction_{0};
  std::chrono::steady_clock::time_point motion_deadline_{};
  std::chrono::steady_clock::time_point last_status_publish_{}, last_io_publish_{};
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ElfinSdkNode>();
  if (!node->connect_sdk()) {rclcpp::shutdown(); return 1;}
  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 2);
  executor.add_node(node); executor.spin(); rclcpp::shutdown(); return 0;
}
