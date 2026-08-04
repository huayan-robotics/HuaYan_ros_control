#include <algorithm>
#include <cctype>
#include <regex>
#include <string>
#include <tuple>

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

bool parse_version(
  const std::string & text, int & major, int & minor, int & patch, int & revision)
{
  std::smatch match;
  const std::regex expression(R"((\d+)\.(\d+)\.(\d+)([A-Za-z]?))");
  if (!std::regex_search(text, match, expression)) {return false;}
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
  const auto expected_model = node->declare_parameter<std::string>("robot_model", "elfin5");
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

  std::string version;
  int cps_version = 0, codesys_version = 0, box_major = 0, box_mid = 0, box_min = 0;
  int algorithm_version = 0, firmware_version = 0;
  const int version_code = HRIF_ReadVersion(
    box_id, robot_id, version, cps_version, codesys_version,
    box_major, box_mid, box_min, algorithm_version, firmware_version);
  if (version_code != 0) {
    RCLCPP_ERROR(node->get_logger(), "Preflight failed: HRIF_ReadVersion returned %d", version_code);
    HRIF_DisConnect(box_id); rclcpp::shutdown(); return 4;
  }

  int major = 0, minor = 0, patch = 0, revision = 0;
  if (!parse_version(version, major, minor, patch, revision)) {
    RCLCPP_ERROR(node->get_logger(),
      "Preflight failed: cannot parse controller version string '%s'", version.c_str());
    HRIF_DisConnect(box_id); rclcpp::shutdown(); return 5;
  }
  const auto actual = std::make_tuple(major, minor, patch, revision);
  const auto minimum = std::make_tuple(6, 5, 20, 4);  // 6.5.20d
  if (actual < minimum) {
    RCLCPP_ERROR(node->get_logger(),
      "Preflight failed: controller version '%s' is older than required version 6.5.20d",
      version.c_str());
    HRIF_DisConnect(box_id); rclcpp::shutdown(); return 6;
  }

  RCLCPP_INFO(node->get_logger(),
    "Preflight passed: model='%s', version='%s' (required >= 6.5.20d)",
    actual_model.c_str(), version.c_str());
  HRIF_DisConnect(box_id);
  rclcpp::shutdown();
  return 0;
}
