#ifndef ELFIN_CONTROLLER_DRIVER__RT_INFO_CLIENT_HPP_
#define ELFIN_CONTROLLER_DRIVER__RT_INFO_CLIENT_HPP_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace elfin_controller_driver
{
struct RtInfo
{
  std::uint32_t frame_size{0}, payload_size{0};
  std::array<std::uint16_t, 8> box_ci{}, box_co{}, box_di{}, box_do{};
  std::array<std::int32_t, 2> analog_output_mode{};
  std::array<double, 2> analog_output{}, analog_input{};
  std::int32_t controller_state{0};
  std::int32_t enabled{0}, paused{0}, moving{0}, blending_done{0}, in_position{0};
  std::int32_t error_axis{0}, error_code{0};
  std::array<std::int32_t, 10> brake_state{};
  std::int32_t axis_group_state{0}, axis_group_error{0};
  std::array<std::int32_t, 10> axis_error{};
  std::array<std::int32_t, 4> end_di{}, end_do{}, end_buttons{};
  std::int32_t end_buttons_enabled{0};
  std::array<double, 2> end_ai{};
};

class RtInfoClient
{
public:
  enum class ReceiveResult {OK, TIMEOUT, DISCONNECTED};
  ~RtInfoClient();
  bool connect(const std::string & host, std::uint16_t port, int timeout_ms);
  void interrupt();
  void disconnect();
  ReceiveResult receive(RtInfo & info, std::string & error);

private:
  bool extract_frame(RtInfo & info, std::string & error);
  int fd_{-1};
  std::vector<std::uint8_t> buffer_;
};
}  // namespace elfin_controller_driver
#endif
