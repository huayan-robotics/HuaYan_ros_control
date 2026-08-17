cmake_minimum_required(VERSION 3.8)
project(elfin_controller_driver)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(ament_cmake REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(controller_manager_msgs REQUIRED)
find_package(elfin_robot_msgs REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(std_srvs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(jsoncpp REQUIRED)

add_library(elfin_controller_hardware SHARED
  src/controller_client.cpp
  src/controller_hardware.cpp)
target_include_directories(elfin_controller_hardware PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
ament_target_dependencies(elfin_controller_hardware
  elfin_robot_msgs hardware_interface pluginlib rclcpp rclcpp_lifecycle sensor_msgs std_srvs)

add_executable(elfin_sdk_node src/sdk_node.cpp src/rt_info_client.cpp)
target_include_directories(elfin_sdk_node PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(elfin_sdk_node
  ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libHR_Pro.so
  JsonCpp::JsonCpp)
ament_target_dependencies(elfin_sdk_node
  controller_manager_msgs elfin_robot_msgs rclcpp sensor_msgs std_srvs)

add_executable(elfin_sdk_preflight src/sdk_preflight.cpp)
target_include_directories(elfin_sdk_preflight PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(elfin_sdk_preflight ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libHR_Pro.so)
ament_target_dependencies(elfin_sdk_preflight rclcpp)

pluginlib_export_plugin_description_file(hardware_interface elfin_controller_hardware.xml)
install(TARGETS elfin_controller_hardware
  ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
install(TARGETS elfin_sdk_node DESTINATION lib/${PROJECT_NAME})
install(TARGETS elfin_sdk_preflight DESTINATION lib/${PROJECT_NAME})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/libHR_Pro.so DESTINATION lib)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../include/HR_Pro.h DESTINATION include)
install(DIRECTORY include/ DESTINATION include)
install(FILES elfin_controller_hardware.xml DESTINATION share/${PROJECT_NAME})
install(FILES README.md DESTINATION share/${PROJECT_NAME})
ament_export_include_directories(include)
ament_export_libraries(elfin_controller_hardware)
ament_export_dependencies(hardware_interface pluginlib rclcpp)
ament_package()
