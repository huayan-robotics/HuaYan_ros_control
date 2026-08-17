#include <algorithm>
#include <arpa/inet.h>
#include <cctype>
#include <cerrno>
#include <cstring>
#include <iterator>
#include <regex>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <tuple>
#include <unistd.h>

#include <rclcpp/rclcpp.hpp>

#include "HR_Pro.h"

namespace
{
std::string normalize_model(const std::string & value)
{
  std::string result;
  for (const unsigned char c : value) {
    if (std::isalnum(c)) {result.push_back(static_cast<char>(std::tolower(c)));}
  }
  return result;
}

bool read_product_version(
  const std::string & host, int port, unsigned int robot_id,
  std::string & product_version, std::string & error)
{
  const int fd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    error = std::string("socket failed: ") + std::strerror(errno);
    return false;
  }
  const timeval timeout{2, 0};
  (void)::setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
  (void)::setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = htons(static_cast<uint16_t>(port));
  if (::inet_pton(AF_INET, host.c_str(), &address.sin_addr) != 1 ||
    ::connect(fd, reinterpret_cast<sockaddr *>(&address), sizeof(address)) != 0)
  {
    error = std::string("connect failed: ") + std::strerror(errno);
    ::close(fd);
    return false;
  }

  const std::string request = "ReadVersion," + std::to_string(robot_id) + ",;";
  std::size_t sent = 0;
  while (sent < request.size()) {
    const ssize_t count = ::send(fd, request.data() + sent, request.size() - sent, 0);
    if (count <= 0) {
      error = std::string("send failed: ") + std::strerror(errno);
      ::close(fd);
      return false;
    }
    sent += static_cast<std::size_t>(count);
  }

  std::string response;
  char buffer[1024];
  while (response.size() < 16384 && response.find(",;") == std::string::npos) {
    const ssize_t count = ::recv(fd, buffer, sizeof(buffer), 0);
    if (count <= 0) {break;}
    response.append(buffer, static_cast<std::size_t>(count));
  }
  ::close(fd);

  std::string compact_response;
  std::copy_if(response.begin(), response.end(), std::back_inserter(compact_response),
    [](const unsigned char c) {return !std::isspace(c);});
  if (compact_response.rfind("ReadVersion,OK,", 0) != 0) {
    error = "unexpected response: '" + response + "'";
    return false;
  }
  const std::regex product_expression(R"((?:^|,)(HR[^,;]+)(?:,|;))",
    std::regex::icase);
  std::smatch match;
  if (!std::regex_search(compact_response, match, product_expression)) {
    error = "product version field is missing from response: '" + response + "'";
    return false;
  }
  product_version = match[1].str();
  return true;
}

bool parse_version(
  const std::string & text, int & major, int & minor, int & patch, int & revision)
{
  std::smatch match;
  // Accept only the prefixed product-version field, never the internal build tuple.
  const std::regex expression(
    R"(^HR(\d+)\.(\d+)\.?([0-9]+)([A-Za-z]?)[0-9]*(?:[._].*)?$)",
    std::regex::icase);
  if (!std::regex_match(text, match, expression)) {return false;}
  major = std::stoi(match[1].str());
  minor = std::stoi(match[2].str());
  patch = std::stoi(match[3].str());
  revision = match[4].str().empty() ? 0 :
    std::tolower(static_cast<unsigned char>(match[4].str()[0])) - 'a' + 1;
  return true;
}
}  // namespace

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<rclcpp::Node>("elfin_sdk_preflight");
  const auto robot_ip = node->declare_parameter<std::string>("robot_ip", "10.20.200.3");
  const auto expected_model = node->declare_parameter<std::string>("robot_model", "E05");
  const int sdk_port = static_cast<int>(node->declare_parameter<int>("sdk_port", 10003));
  const auto box_id = static_cast<unsigned int>(node->declare_parameter<int>("box_id", 0));
  const auto robot_id = static_cast<unsigned int>(node->declare_parameter<int>("robot_id", 0));

  const int connect_code = HRIF_Connect(
    box_id, robot_ip.c_str(), static_cast<unsigned short>(sdk_port));
  if (connect_code != 0) {
    RCLCPP_ERROR(node->get_logger(), "Preflight failed: SDK connection to %s:%d returned %d",
      robot_ip.c_str(), sdk_port, connect_code);
    rclcpp::shutdown();
    return 1;
  }

  std::string actual_model;
  const int model_code = HRIF_ReadRobotModel(box_id, actual_model);
  if (model_code != 0) {
    RCLCPP_ERROR(node->get_logger(), "Preflight failed: HRIF_ReadRobotModel returned %d", model_code);
    HRIF_DisConnect(box_id); rclcpp::shutdown(); return 2;
  }
  if (normalize_model(actual_model) != normalize_model(expected_model)) {
    RCLCPP_ERROR(node->get_logger(),
      "Preflight failed: controller model '%s' does not match launch robot_model '%s'",
      actual_model.c_str(), expected_model.c_str());
    HRIF_DisConnect(box_id); rclcpp::shutdown(); return 3;
  }

  std::string internal_version;
  int cps_version = 0, codesys_version = 0, box_major = 0, box_mid = 0, box_min = 0;
  int algorithm_version = 0, firmware_version = 0;
  const int version_code = HRIF_ReadVersion(
    box_id, robot_id, internal_version, cps_version, codesys_version,
    box_major, box_mid, box_min, algorithm_version, firmware_version);
  if (version_code != 0) {
    RCLCPP_ERROR(node->get_logger(), "Preflight failed: HRIF_ReadVersion returned %d", version_code);
    HRIF_DisConnect(box_id); rclcpp::shutdown(); return 4;
  }
  HRIF_DisConnect(box_id);

  std::string version;
  std::string raw_version_error;
  if (!read_product_version(
      robot_ip, sdk_port, robot_id, version, raw_version_error))
  {
    RCLCPP_ERROR(node->get_logger(),
      "Preflight failed: cannot read controller product version: %s",
      raw_version_error.c_str());
    rclcpp::shutdown(); return 5;
  }

  int major = 0, minor = 0, patch = 0, revision = 0;
  if (!parse_version(version, major, minor, patch, revision)) {
    RCLCPP_ERROR(node->get_logger(),
      "Preflight failed: cannot parse controller version string '%s'", version.c_str());
    rclcpp::shutdown(); return 5;
  }
  const auto actual = std::make_tuple(major, minor, patch, revision);
  const auto minimum = std::make_tuple(6, 5, 20, 4);  // 6.5.20d
  if (actual < minimum) {
    RCLCPP_ERROR(node->get_logger(),
      "Preflight failed: controller version '%s' is older than required version 6.5.20d",
      version.c_str());
    rclcpp::shutdown(); return 6;
  }

  RCLCPP_INFO(node->get_logger(),
    "Preflight passed: model='%s', version='%s' (required >= 6.5.20d), "
    "internal_version='%s'",
    actual_model.c_str(), version.c_str(), internal_version.c_str());
  rclcpp::shutdown();
  return 0;
}
