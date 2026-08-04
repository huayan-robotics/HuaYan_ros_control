#ifndef ELFIN_CONTROLLER_DRIVER__CONTROLLER_CLIENT_HPP_
#define ELFIN_CONTROLLER_DRIVER__CONTROLLER_CLIENT_HPP_

#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <mutex>
#include <string>

namespace elfin_controller_driver
{
constexpr std::size_t kJointCount = 6;

struct RobotState
{
  std::array<double, kJointCount> target_position{};
  std::array<double, kJointCount> target_velocity{};
  std::array<double, kJointCount> actual_position{};
  std::array<double, kJointCount> actual_velocity{};
  std::array<double, kJointCount> actual_effort{};
  std::array<double, kJointCount> target_tcp_position{};
  std::array<double, kJointCount> target_tcp_velocity{};
  std::array<double, kJointCount> tcp_position{};
  std::array<double, kJointCount> tcp_velocity{};
  std::array<double, kJointCount> force{};
  std::array<double, kJointCount> frame_force{};
  double speed_scaling{0.0};
  std::uint64_t controller_time_us{0};
  std::int32_t state_machine{0};
  std::int32_t force_control_state{0};
  std::chrono::steady_clock::time_point received_at{};
};

class StateClient
{
public:
  enum class ReceiveResult {OK, TIMEOUT, DISCONNECTED, INVALID_FRAME};
  ~StateClient();
  bool connect(const std::string & host, std::uint16_t port, int timeout_ms);
  void interrupt();
  void disconnect();
  ReceiveResult receive(RobotState & state, std::string & error);
  bool connected() const noexcept {return fd_ >= 0;}
private:
  ReceiveResult receive_frame(std::string & error);
  int fd_{-1};
  std::array<std::uint8_t, 1136> frame_{};
  std::size_t frame_bytes_{0};
};

class RealtimeClient
{
public:
  ~RealtimeClient();
  bool connect(const std::string & host, std::uint16_t port, int timeout_ms);
  void disconnect();
  bool start_servo(double gain, double lookahead_s);
  bool servo_j(const std::array<double, kJointCount> & degrees);
  bool speed_j(const std::array<double, kJointCount> & degrees_per_second,
    double acceleration, double runtime_s);
  bool stop_speed();
  bool connected() const noexcept {return fd_ >= 0;}
  const std::string & last_error() const noexcept {return last_error_;}
private:
  bool send_command(const std::string & command);
  int fd_{-1};
  std::string last_error_;
  std::mutex send_mutex_;
};
}  // namespace elfin_controller_driver
#endif
