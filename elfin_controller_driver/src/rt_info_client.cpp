#include "elfin_controller_driver/rt_info_client.hpp"

#include <arpa/inet.h>
#include <algorithm>
#include <cerrno>
#include <cstring>
#include <memory>
#include <netinet/tcp.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

#include <json/json.h>

namespace elfin_controller_driver
{
namespace
{
constexpr std::uint32_t kMagic = 0x5242544cU;
constexpr std::uint32_t kMaximumFrameSize = 1024U * 1024U;

template<typename T>
T read_at(const std::uint8_t * data, std::size_t offset)
{
  T value{};
  std::memcpy(&value, data + offset, sizeof(T));
  return value;
}

double json_number(const Json::Value & value)
{
  if (value.isString()) {return std::stod(value.asString());}
  return value.asDouble();
}

template<typename T, std::size_t N>
void json_array(const Json::Value & value, std::array<T, N> & output)
{
  if (!value.isArray()) {throw std::runtime_error("expected JSON array");}
  const auto count = std::min<std::size_t>(N, value.size());
  for (std::size_t i = 0; i < count; ++i) {
    output[i] = static_cast<T>(json_number(value[static_cast<Json::ArrayIndex>(i)]));
  }
}

void parse_payload(const std::uint8_t * payload, std::size_t size, RtInfo & info)
{
  Json::CharReaderBuilder builder;
  builder["collectComments"] = false;
  Json::Value root;
  std::string parse_error;
  const char * begin = reinterpret_cast<const char *>(payload);
  std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
  if (!reader->parse(begin, begin + size, &root, &parse_error)) {
    throw std::runtime_error("invalid JSON: " + parse_error);
  }

  const auto & end_io = root["EndIO"];
  json_array(end_io["EndDI"], info.end_di);
  json_array(end_io["EndDO"], info.end_do);
  json_array(end_io["EndButton"], info.end_buttons);
  info.end_buttons_enabled = end_io["EnableEndBTN"].asInt();
  json_array(end_io["EndAI"], info.end_ai);

  const auto & box_io = root["ElectricBoxIO"];
  json_array(box_io["BoxCI"], info.box_ci);
  json_array(box_io["BoxCO"], info.box_co);
  json_array(box_io["BoxDI"], info.box_di);
  json_array(box_io["BoxDO"], info.box_do);

  const auto & analog = root["ElectricBoxAnalogIO"];
  for (std::size_t i = 0; i < 2; ++i) {
    const std::string suffix = std::to_string(i + 1);
    info.analog_output_mode[i] = analog["BoxAnalogOutMode_" + suffix].asInt();
    info.analog_output[i] = json_number(analog["BoxAnalogOut_" + suffix]);
    info.analog_input[i] = json_number(analog["BoxAnalogIn_" + suffix]);
  }

  const auto & state = root["StateAndError"];
  info.controller_state = state["robotState"].asInt();
  info.enabled = state["robotEnabled"].asInt();
  info.paused = state["robotPaused"].asInt();
  info.moving = state["robotMoving"].asInt();
  info.blending_done = state["robotBlendingDone"].asInt();
  info.in_position = state["InPos"].asInt();
  info.error_axis = state["Error_AxisID"].asInt();
  info.error_code = state["Error_Code"].asInt();
  json_array(state["BrakeState"], info.brake_state);
  json_array(state["nAxisErrorCode"], info.axis_error);
  if (state["nAxisGroupStatus"].isArray() && !state["nAxisGroupStatus"].empty()) {
    info.axis_group_state = state["nAxisGroupStatus"][0].asInt();
  }
  if (state["nAxisGroupErrorCode"].isArray() && !state["nAxisGroupErrorCode"].empty()) {
    info.axis_group_error = state["nAxisGroupErrorCode"][0].asInt();
  }
}

int open_socket(const std::string & host, std::uint16_t port, int timeout_ms)
{
  const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {return -1;}
  timeval timeout{timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  ::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
  int enabled = 1;
  ::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enabled, sizeof(enabled));
  ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &enabled, sizeof(enabled));
#ifdef TCP_KEEPIDLE
  int keepidle = 2; ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
#endif
#ifdef TCP_KEEPINTVL
  int keepintvl = 1; ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
#endif
#ifdef TCP_KEEPCNT
  int keepcnt = 3; ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
#endif
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  if (::inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1 ||
      ::connect(fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != 0) {
    ::close(fd); return -1;
  }
  return fd;
}
}  // namespace

RtInfoClient::~RtInfoClient() {disconnect();}
bool RtInfoClient::connect(const std::string & host, std::uint16_t port, int timeout_ms)
{
  disconnect(); buffer_.clear(); fd_ = open_socket(host, port, timeout_ms); return fd_ >= 0;
}
void RtInfoClient::interrupt() {if (fd_ >= 0) {::shutdown(fd_, SHUT_RDWR);}}
void RtInfoClient::disconnect()
{
  if (fd_ >= 0) {
    ::shutdown(fd_, SHUT_RDWR);
    ::close(fd_);
    fd_ = -1;
  }
  buffer_.clear();
}

bool RtInfoClient::extract_frame(RtInfo & info, std::string & error)
{
  static constexpr std::array<std::uint8_t, 4> magic_bytes{{0x4c, 0x54, 0x42, 0x52}};
  auto magic = std::search(buffer_.begin(), buffer_.end(), magic_bytes.begin(), magic_bytes.end());
  if (magic != buffer_.begin()) {
    if (magic == buffer_.end()) {
      if (buffer_.size() > 3) {buffer_.erase(buffer_.begin(), buffer_.end() - 3);}
      return false;
    }
    buffer_.erase(buffer_.begin(), magic);
  }
  if (buffer_.size() < 12) {return false;}
  const auto total_size = read_at<std::uint32_t>(buffer_.data(), 4);
  const auto data_size = read_at<std::uint32_t>(buffer_.data(), 8);
  if (read_at<std::uint32_t>(buffer_.data(), 0) != kMagic ||
      total_size != 12U + data_size || data_size == 0U ||
      total_size > kMaximumFrameSize) {
    error = "invalid 10004 frame header: total_size=" + std::to_string(total_size) +
      ", data_size=" + std::to_string(data_size);
    buffer_.erase(buffer_.begin());
    return false;
  }
  if (buffer_.size() < total_size) {return false;}
  const auto * payload = buffer_.data() + 12;
  info.frame_size = total_size;
  info.payload_size = data_size;
  try {
    parse_payload(payload, data_size, info);
  } catch (const std::exception & exception) {
    error = "invalid 10004 JSON payload: " + std::string(exception.what());
    buffer_.erase(buffer_.begin(), buffer_.begin() + total_size);
    return false;
  }
  buffer_.erase(buffer_.begin(), buffer_.begin() + total_size);
  error.clear();
  return true;
}

RtInfoClient::ReceiveResult RtInfoClient::receive(RtInfo & info, std::string & error)
{
  while (true) {
    if (extract_frame(info, error)) {return ReceiveResult::OK;}
    if (!error.empty()) {
      disconnect();
      return ReceiveResult::DISCONNECTED;
    }
    std::array<std::uint8_t, 8192> chunk{};
    const auto count = ::recv(fd_, chunk.data(), chunk.size(), 0);
    if (count > 0) {
      buffer_.insert(buffer_.end(), chunk.begin(), chunk.begin() + count);
      continue;
    }
    if (count < 0 && errno == EINTR) {continue;}
    if (count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return ReceiveResult::TIMEOUT;
    }
    error = count == 0 ? "controller closed 10004 connection" : std::strerror(errno);
    disconnect();
    return ReceiveResult::DISCONNECTED;
  }
}
}  // namespace elfin_controller_driver
