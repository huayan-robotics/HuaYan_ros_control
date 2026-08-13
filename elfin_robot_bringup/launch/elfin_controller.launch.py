#!/usr/bin/env python3
"""Start an Elfin controller-backed ros2_control system."""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    EmitEvent,
    IncludeLaunchDescription,
    LogInfo,
    OpaqueFunction,
    RegisterEventHandler,
)
from launch.event_handlers import OnProcessExit
from launch.events import Shutdown
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

MODEL_RESOURCES = {
    "E03": "elfin3",
    "E05": "elfin5",
    "E05-L": "elfin5_l",
    "E10": "elfin10",
    "E10-L": "elfin10_l",
    "E15": "elfin15",
}


def _launch(context):
    model = LaunchConfiguration("robot_model").perform(context)
    mode = LaunchConfiguration("control_mode").perform(context)
    validation_value = LaunchConfiguration("enable_controller_validation").perform(context).lower()
    if model not in MODEL_RESOURCES:
        raise RuntimeError(f"Unsupported robot_model: {model}")
    if mode not in {"position", "velocity", "controller"}:
        raise RuntimeError(f"Unsupported control_mode: {mode}")
    if validation_value not in {"true", "false", "1", "0", "yes", "no", "on", "off"}:
        raise RuntimeError("enable_controller_validation must be true or false")
    validation_enabled = validation_value in {"true", "1", "yes", "on"}
    update_rate_text = LaunchConfiguration("update_rate").perform(context)
    try:
        update_rate = int(update_rate_text)
    except ValueError as exception:
        raise RuntimeError("update_rate must be 250 or 1000") from exception
    if update_rate not in {250, 1000}:
        raise RuntimeError("update_rate must be 250 or 1000")
    resource_model = MODEL_RESOURCES[model]

    model_package = f"{resource_model}_ros2_gazebo"
    xacro_file = os.path.join(
        get_package_share_directory(model_package), "urdf", f"{resource_model}.urdf.xacro")
    bringup_share = get_package_share_directory("elfin_robot_bringup")
    config_suffix = "state_only" if mode == "controller" else mode
    controllers = os.path.join(bringup_share, "config", f"elfin_controller_{config_suffix}.yaml")
    moveit_launch = os.path.join(bringup_share, "launch", "elfin_moveit.launch.py")

    robot_description = {
        "robot_description": [
            "<?xml version='1.0'?><robot name='elfin_controller'>",
            "<ros2_control name='ElfinController' type='system'><hardware>",
            "<plugin>elfin_controller_driver/ElfinControllerHardware</plugin>",
            "<param name='robot_ip'>", LaunchConfiguration("robot_ip"), "</param>",
            "<param name='state_port'>8893</param><param name='command_port'>8892</param>",
            "<param name='state_stale_timeout_ms'>", LaunchConfiguration("state_stale_timeout_ms"), "</param>",
            "<param name='state_disconnect_timeout_ms'>", LaunchConfiguration("state_disconnect_timeout_ms"), "</param>",
            "<param name='controller_joint_unit'>degree</param>",
            "<param name='servo_gain'>", LaunchConfiguration("servo_gain"), "</param>",
            "<param name='lookahead_time'>", LaunchConfiguration("lookahead_time"), "</param>",
            "<param name='position_command_epsilon'>", LaunchConfiguration("position_command_epsilon"), "</param>",
            "<param name='velocity_command_epsilon'>", LaunchConfiguration("velocity_command_epsilon"), "</param>",
            "<param name='command_log_throttle_ms'>", LaunchConfiguration("command_log_throttle_ms"), "</param>",
            "<param name='servo_restart_idle_ms'>", LaunchConfiguration("servo_restart_idle_ms"), "</param>",
            "<param name='loop_diagnostics'>", LaunchConfiguration("loop_diagnostics"), "</param>",
            "<param name='loop_diagnostics_period'>", LaunchConfiguration("loop_diagnostics_period"), "</param>",
            "<param name='expected_update_rate'>", str(update_rate), "</param>",
            "</hardware>",
            "".join(
                f"<joint name='elfin_joint{i}'>"
                "<command_interface name='position'/><command_interface name='velocity'/>"
                "<state_interface name='position'/><state_interface name='velocity'/>"
                "<state_interface name='effort'/></joint>" for i in range(1, 7)),
            "</ros2_control></robot>",
        ]
    }

    # The controller manager only needs ros2_control metadata. The full model is
    # still published from the model xacro so RViz and MoveIt receive all links.
    from launch.substitutions import Command, FindExecutable
    full_description = {"robot_description": Command([FindExecutable(name="xacro"), " ", xacro_file,
        " use_real_hardware:=false use_fake_hardware:=false"])}

    runtime_nodes = [
        Node(package="robot_state_publisher", executable="robot_state_publisher",
             parameters=[full_description], output="screen"),
        Node(package="controller_manager", executable="ros2_control_node",
             parameters=[robot_description, controllers, {"update_rate": update_rate}],
             output="screen",
             on_exit=EmitEvent(event=Shutdown(reason="ros2_control exited"))),
        Node(package="controller_manager", executable="spawner",
             arguments=["joint_state_broadcaster", "--controller-manager", "/controller_manager"],
             output="screen"),
        Node(package="elfin_controller_driver", executable="elfin_sdk_node",
             parameters=[{"robot_ip": LaunchConfiguration("robot_ip"),
                          "state_port": LaunchConfiguration("pushed_state_port"),
                          "state_socket_timeout_ms": LaunchConfiguration("pushed_state_socket_timeout_ms"),
                          "state_disconnect_timeout_ms": LaunchConfiguration("pushed_state_disconnect_timeout_ms"),
                          "status_publish_rate": LaunchConfiguration("status_publish_rate"),
                          "io_publish_rate": LaunchConfiguration("io_publish_rate"),
                          "auto_start_ros_control": mode in {"position", "velocity"},
                          "motion_controller": "elfin_velocity_controller" if mode == "velocity" else "elfin_arm_controller"}],
             output="screen", respawn=True, respawn_delay=2.0),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(moveit_launch),
            launch_arguments={
                "robot_model": model,
                "use_sim_time": "false",
            }.items(),
        ),
    ]
    if mode == "position":
        runtime_nodes.append(Node(package="controller_manager", executable="spawner",
            arguments=["elfin_arm_controller", "--inactive", "--controller-manager", "/controller_manager"],
            output="screen"))
    elif mode == "velocity":
        runtime_nodes.append(Node(package="controller_manager", executable="spawner",
            arguments=["elfin_velocity_controller", "--inactive", "--controller-manager", "/controller_manager"],
            output="screen"))
    else:
        # Keep the trajectory controller loaded but inactive so the SDK control
        # ownership service can activate it later without restarting bringup.
        runtime_nodes.append(Node(package="controller_manager", executable="spawner",
            arguments=["elfin_arm_controller", "--inactive", "--controller-manager", "/controller_manager"],
            output="screen"))
    if not validation_enabled:
        return [
            LogInfo(msg="Elfin controller model/version validation is disabled"),
            *runtime_nodes,
        ]

    preflight = Node(
        package="elfin_controller_driver",
        executable="elfin_sdk_preflight",
        parameters=[{
            "robot_ip": LaunchConfiguration("robot_ip"),
            "robot_model": model,
        }],
        output="screen",
    )

    def _after_preflight(event, _context):
        if event.returncode == 0:
            return [LogInfo(msg="Elfin controller preflight passed; starting ros2_control"), *runtime_nodes]
        return [
            LogInfo(msg=f"ERROR: Elfin controller preflight failed with exit code {event.returncode}; ros2_control will not start"),
            EmitEvent(event=Shutdown(reason="Elfin controller model/version validation failed")),
        ]

    return [
        RegisterEventHandler(OnProcessExit(target_action=preflight, on_exit=_after_preflight)),
        preflight,
    ]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument("robot_model", default_value="E05"),
        DeclareLaunchArgument("robot_ip", default_value="10.20.200.3"),
        DeclareLaunchArgument("control_mode", default_value="controller"),
        DeclareLaunchArgument("servo_gain", default_value="8000"),
        DeclareLaunchArgument("lookahead_time", default_value="0.004"),
        DeclareLaunchArgument("state_stale_timeout_ms", default_value="100"),
        DeclareLaunchArgument("state_disconnect_timeout_ms", default_value="1000"),
        DeclareLaunchArgument("position_command_epsilon", default_value="1e-8"),
        DeclareLaunchArgument("velocity_command_epsilon", default_value="1e-8"),
        DeclareLaunchArgument("command_log_throttle_ms", default_value="100"),
        DeclareLaunchArgument("servo_restart_idle_ms", default_value="100"),
        DeclareLaunchArgument("loop_diagnostics", default_value="false"),
        DeclareLaunchArgument("loop_diagnostics_period", default_value="5.0"),
        DeclareLaunchArgument("update_rate", default_value="1000"),
        DeclareLaunchArgument("enable_controller_validation", default_value="true"),
        DeclareLaunchArgument("pushed_state_port", default_value="10004"),
        DeclareLaunchArgument("pushed_state_socket_timeout_ms", default_value="100"),
        DeclareLaunchArgument("pushed_state_disconnect_timeout_ms", default_value="1000"),
        DeclareLaunchArgument("status_publish_rate", default_value="10.0"),
        DeclareLaunchArgument("io_publish_rate", default_value="5.0"),
        OpaqueFunction(function=_launch),
    ])
