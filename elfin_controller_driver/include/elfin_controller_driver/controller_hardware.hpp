#ifndef ELFIN_CONTROLLER_DRIVER__CONTROLLER_HARDWARE_HPP_
#define ELFIN_CONTROLLER_DRIVER__CONTROLLER_HARDWARE_HPP_

#include "elfin_controller_driver/controller_client.hpp"

#include <atomic>
#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <hardware_interface/system_interface.hpp>
#include <hardware_interface/types/hardware_interface_type_values.hpp>
#include <elfin_robot_msgs/msg/elfin_realtime_state.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_srvs/srv/set_bool.hpp>
#include <std_srvs/srv/trigger.hpp>

namespace elfin_controller_driver
{
class ElfinControllerHardware : public hardware_interface::SystemInterface
{
public:
  ~ElfinControllerHardware() override;
  hardware_interface::CallbackReturn on_init(
    const hardware_interface::HardwareInfo & info) override;
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;
  hardware_interface::CallbackReturn on_activate(
    const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::CallbackReturn on_cleanup(
    const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::CallbackReturn on_shutdown(
    const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::CallbackReturn on_error(
    const rclcpp_lifecycle::State & previous_state) override;
  hardware_interface::return_type read(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;
  hardware_interface::return_type write(
    const rclcpp::Time & time, const rclcpp::Duration & period) override;
  hardware_interface::return_type prepare_command_mode_switch(
    const std::vector<std::string> & start, const std::vector<std::string> & stop) override;
  hardware_interface::return_type perform_command_mode_switch(
    const std::vector<std::string> & start, const std::vector<std::string> & stop) override;

private:
  enum class CommandMode {NONE, POSITION, VELOCITY};
  enum class Owner {CONTROLLER, ROS, FREEDRIVE};
  bool connect_clients();
  void disconnect_clients();
  void state_loop();
  void sync_commands();
  void reset_command_baseline();
  bool command_changed(
    const std::vector<double> & command, const std::vector<double> & baseline,
    double epsilon) const;
  void publish_realtime_state(const RobotState & state);
  bool all_interfaces(const std::vector<std::string> & interfaces, const std::string & type) const;
  static double parameter_double(const hardware_interface::HardwareInfo &, const std::string &, double);
  static int parameter_int(const hardware_interface::HardwareInfo &, const std::string &, int);
  static std::string parameter_string(const hardware_interface::HardwareInfo &, const std::string &, const std::string &);

  std::string robot_ip_;
  int state_port_{8893};
  int command_port_{8892};
  int socket_timeout_ms_{1000};
  int state_stale_timeout_ms_{100};
  int state_disconnect_timeout_ms_{1000};
  double servo_gain_{8000.0};
  double lookahead_time_{0.004};
  double speed_acceleration_deg_{180.0};
  double speed_runtime_{0.02};
  double position_command_epsilon_{1e-8};
  double velocity_command_epsilon_{1e-8};
  int servo_restart_idle_ms_{100};
  int command_log_throttle_ms_{100};
  int expected_update_rate_{1000};
  double unit_scale_{3.14159265358979323846 / 180.0};
  std::vector<double> positions_, velocities_, efforts_, position_commands_, velocity_commands_;
  std::vector<double> last_position_command_, last_velocity_command_;
  RobotState latest_state_;
  std::mutex state_mutex_;
  StateClient state_client_;
  RealtimeClient realtime_client_;
  std::thread state_thread_;
  std::atomic<bool> running_{false};
  std::atomic<bool> state_valid_{false};
  std::atomic<bool> have_state_{false};
  std::atomic<bool> resync_commands_requested_{false};
  std::atomic<Owner> owner_{Owner::CONTROLLER};
  CommandMode active_mode_{CommandMode::NONE};
  CommandMode requested_mode_{CommandMode::NONE};
  bool servo_started_{false};
  std::chrono::steady_clock::time_point last_servo_command_at_{};
  double state_publish_rate_{100.0};
  std::chrono::steady_clock::time_point last_state_publish_{};
  rclcpp::Node::SharedPtr state_publisher_node_;
  rclcpp::Clock command_log_clock_{RCL_STEADY_TIME};
  rclcpp::Publisher<elfin_robot_msgs::msg::ElfinRealtimeState>::SharedPtr realtime_publisher_;
  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr joint_publisher_;
};
}  // namespace elfin_controller_driver
#endif
