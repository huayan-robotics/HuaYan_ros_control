cmake_minimum_required(VERSION 3.5)
project(elfin_robot_msgs)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

find_package(rosidl_default_generators REQUIRED)
find_package(std_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/ElfinIODRead.srv"
  "srv/ElfinIODWrite.srv"
  "srv/SetFloat64.srv"
  "srv/SetInt16.srv"
  "srv/SetString.srv"
  "srv/SetPose.srv"
  "srv/ConfigureTcp.srv"
  "srv/SetDigitalIO.srv"
  "srv/SetAnalogIO.srv"
  "srv/Jog.srv"
  "srv/MoveTarget.srv"
  "srv/GetInt32.srv"
  "srv/SetPayload.srv"
  "srv/GetPayload.srv"
  "srv/GetPose.srv"
  "srv/GetTcpConfig.srv"
  "srv/SetBrake.srv"
  "msg/ElfinRealtimeState.msg"
  "msg/ElfinRobotStatus.msg"
  "msg/ElfinIOState.msg"
  "msg/ElfinEndIOState.msg"
  "msg/ElfinBrakeState.msg"
  DEPENDENCIES std_msgs
 )

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
