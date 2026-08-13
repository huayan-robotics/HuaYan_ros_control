#include "elfin_controller_driver/controller_client.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <netinet/tcp.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

namespace elfin_controller_driver
{
namespace
{
constexpr std::uint32_t kMagic = 0x5242544cU;
constexpr std::size_t kFrameSize = 1136;

template<typename T>
T read_le(const std::uint8_t *& cursor)
{
  T value{};
  std::memcpy(&value, cursor, sizeof(T));
  cursor += sizeof(T);
  return value;
}

void read_six_doubles(const std::uint8_t *& cursor, std::array<double, 6> & output)
{
  for (auto & value : output) {value = read_le<double>(cursor);}
}

int open_socket(const std::string & host, std::uint16_t port, int timeout_ms)
{
  const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {return -1;}
  timeval timeout{timeout_ms / 1000, (timeout_ms % 1000) * 1000};
  ::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
  ::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
  int keepalive = 1;
  ::setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
#ifdef TCP_KEEPIDLE
  int keepidle = 2;
  ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
#endif
#ifdef TCP_KEEPINTVL
  int keepintvl = 1;
  ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
#endif
#ifdef TCP_KEEPCNT
  int keepcnt = 3;
  ::setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
#endif
  int no_delay = 1;
  ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &no_delay, sizeof(no_delay));
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  if (::inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1 ||
      ::connect(fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != 0)
  {
    const int connection_error = errno == 0 ? EINVAL : errno;
    ::close(fd);
    errno = connection_error;
    return -1;
  }
  return fd;
}
}  // namespace

StateClient::~StateClient() {disconnect();}
bool StateClient::connect(const std::string & host, std::uint16_t port, int timeout_ms)
{
  disconnect();
  frame_bytes_ = 0;
  fd_ = open_socket(host, port, timeout_ms);
  return fd_ >= 0;
}
void StateClient::disconnect()
{
  if (fd_ >= 0) {
    ::shutdown(fd_, SHUT_RDWR);
    ::close(fd_);
    fd_ = -1;
  }
}
void StateClient::interrupt()
{
  if (fd_ >= 0) {::shutdown(fd_, SHUT_RDWR);}
}
StateClient::ReceiveResult StateClient::receive_frame(std::string & error)
{
  while (frame_bytes_ < frame_.size()) {
    const auto count = ::recv(
      fd_, frame_.data() + frame_bytes_, frame_.size() - frame_bytes_, 0);
    if (count > 0) {
      frame_bytes_ += static_cast<std::size_t>(count);
      continue;
    }
    if (count < 0 && errno == EINTR) {continue;}
    // Preserve a partial TCP frame across ordinary socket timeouts.
    if (count < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      return ReceiveResult::TIMEOUT;
    }
    error = count == 0 ? "controller closed state connection" : std::strerror(errno);
    return ReceiveResult::DISCONNECTED;
  }
  return ReceiveResult::OK;
}
StateClient::ReceiveResult StateClient::receive(RobotState & state, std::string & error)
{
  const auto result = receive_frame(error);
  if (result != ReceiveResult::OK) {
    if (result == ReceiveResult::DISCONNECTED) {disconnect();}
    return result;
  }
  const std::uint8_t * cursor = frame_.data();
  const auto magic = read_le<std::uint32_t>(cursor);
  const auto message_size = read_le<std::int32_t>(cursor);
  const auto data_size = read_le<std::int32_t>(cursor);
  state.cycle_time_ms = read_le<std::int32_t>(cursor);
  if (magic != kMagic || message_size != static_cast<std::int32_t>(kFrameSize) || data_size != 1120) {
    std::ostringstream details;
    details << "invalid 8893 frame header: magic=0x" << std::hex << magic << std::dec
            << ", message_size=" << message_size << ", data_size=" << data_size
            << " (expected magic=0x" << std::hex << kMagic << std::dec
            << ", message_size=" << kFrameSize << ", data_size=1120)";
    error = details.str();
    frame_bytes_ = 0;
    disconnect();
    return ReceiveResult::INVALID_FRAME;
  }
  read_six_doubles(cursor, state.target_position);
  read_six_doubles(cursor, state.target_velocity);
  cursor += 3 * 6 * sizeof(double);  // target acceleration/current/torque
  read_six_doubles(cursor, state.actual_position);
  read_six_doubles(cursor, state.actual_velocity);
  cursor += 2 * 6 * sizeof(double);  // actual acceleration/current
  read_six_doubles(cursor, state.actual_effort);
  read_six_doubles(cursor, state.target_tcp_position);
  read_six_doubles(cursor, state.target_tcp_velocity);
  read_six_doubles(cursor, state.tcp_position);
  read_six_doubles(cursor, state.tcp_velocity);
  cursor += 2 * 6 * sizeof(double);  // command UCS/TCP
  cursor += 2 * sizeof(double);      // TCP command/actual scalar velocity
  read_six_doubles(cursor, state.force);
  read_six_doubles(cursor, state.frame_force);
  state.speed_scaling = read_le<double>(cursor);
  cursor += 3 * sizeof(double);      // momentum/physical/electric power
  state.controller_time_us = read_le<std::uint64_t>(cursor);
  state.state_machine = read_le<std::int32_t>(cursor);
  state.force_control_state = read_le<std::int32_t>(cursor);
  state.received_at = std::chrono::steady_clock::now();
  frame_bytes_ = 0;
  return ReceiveResult::OK;
}

RealtimeClient::~RealtimeClient() {disconnect();}
bool RealtimeClient::connect(const std::string & host, std::uint16_t port, int timeout_ms)
{
  disconnect();
  fd_ = open_socket(host, port, timeout_ms);
  last_error_ = fd_ >= 0 ? std::string{} : std::strerror(errno);
  return fd_ >= 0;
}
void RealtimeClient::disconnect()
{
  if (fd_ >= 0) {
    // Send a TCP FIN before releasing the descriptor. This makes the controller
    // release its single-client 8892 session immediately on ROS shutdown or a
    // control-ownership switch.
    ::shutdown(fd_, SHUT_RDWR);
    ::close(fd_);
    fd_ = -1;
  }
}
bool RealtimeClient::send_command(const std::string & command)
{
  std::lock_guard<std::mutex> lock(send_mutex_);
  std::size_t done = 0;
  while (done < command.size()) {
    const auto count = ::send(fd_, command.data() + done, command.size() - done, MSG_NOSIGNAL);
    if (count <= 0) {
      last_error_ = count == 0 ? "socket closed" : std::strerror(errno);
      disconnect();
      return false;
    }
    done += static_cast<std::size_t>(count);
  }
  return true;
}
bool RealtimeClient::start_servo(double gain, double lookahead_s)
{
  std::ostringstream ss; ss << std::setprecision(12) << "StartServo,0," << gain << ',' << lookahead_s << ",;";
  return send_command(ss.str());
}
bool RealtimeClient::servo_j(const std::array<double, 6> & q)
{
  std::ostringstream ss; ss << std::setprecision(12) << "PushServoJ,0";
  for (const auto value : q) {ss << ',' << value;} ss << ",;"; return send_command(ss.str());
}
bool RealtimeClient::speed_j(const std::array<double, 6> & qd, double acceleration, double runtime_s)
{
  std::ostringstream ss; ss << std::setprecision(12) << "SpeedJ,0";
  for (const auto value : qd) {ss << ',' << value;}
  ss << ',' << acceleration << ',' << runtime_s << ",;"; return send_command(ss.str());
}
bool RealtimeClient::stop_speed() {return speed_j({}, 180.0, 0.02);}
}  // namespace elfin_controller_driver
