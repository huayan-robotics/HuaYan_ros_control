#!/usr/bin/env python3
"""Unified Elfin bringup entry point for simulation and real controllers."""

import os
import yaml

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription, OpaqueFunction, SetLaunchConfiguration
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration


MODEL_RESOURCES = {
    "E03": "elfin3",
    "E05": "elfin5",
    "E05-L": "elfin5_l",
    "E10": "elfin10",
    "E10-L": "elfin10_l",
    "E15": "elfin15",
}

CONFIG_KEYS = {
    "robot_model",
    "hardware_type",
    "robot_ip",
    "control_mode",
    "servo_gain",
    "lookahead_time",
    "state_stale_timeout_ms",
    "state_disconnect_timeout_ms",
    "position_command_epsilon",
    "velocity_command_epsilon",
    "command_log_throttle_ms",
    "servo_restart_idle_ms",
    "enable_controller_validation",
    "pushed_state_port",
    "pushed_state_socket_timeout_ms",
    "pushed_state_disconnect_timeout_ms",
    "status_publish_rate",
    "io_publish_rate",
    "update_rate",
}


def _load_config(context):
    config_file = LaunchConfiguration("config_file").perform(context)
    if not os.path.isfile(config_file):
        raise RuntimeError(f"Elfin launch configuration file not found: {config_file}")
    with open(config_file, "r", encoding="utf-8") as stream:
        document = yaml.safe_load(stream) or {}
    values = document.get("elfin_control", document)
    if not isinstance(values, dict):
        raise RuntimeError("Elfin launch configuration must be a YAML mapping")
    unknown = sorted(set(values) - CONFIG_KEYS)
    if unknown:
        raise RuntimeError(f"Unknown Elfin launch configuration keys: {', '.join(unknown)}")

    actions = []
    for key, value in values.items():
        # Explicit command-line values remain available as an optional
        # one-shot override; otherwise YAML supplies the launch value.
        if key in context.launch_configurations:
            continue
        if value is None:
            raise RuntimeError(f"Elfin launch configuration '{key}' cannot be null")
        if isinstance(value, bool):
            value = "true" if value else "false"
        actions.append(SetLaunchConfiguration(key, str(value)))
    return actions


def _configure(context):
    model = LaunchConfiguration("robot_model").perform(context)
    hardware_type = LaunchConfiguration("hardware_type").perform(context)

    if model not in MODEL_RESOURCES:
        raise RuntimeError(
            f"Unsupported robot_model '{model}'. "
            f"Choose one of: {', '.join(MODEL_RESOURCES)}"
        )
    resource_model = MODEL_RESOURCES[model]

    if hardware_type == "gazebo":
        simulation_package = f"{resource_model}_ros2_moveit2"
        simulation_launch = os.path.join(
            get_package_share_directory(simulation_package),
            "launch",
            f"{resource_model}.launch.py",
        )
        return [
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(simulation_launch)
            )
        ]

    if hardware_type == "controller":
        controller_launch = os.path.join(
            get_package_share_directory("elfin_robot_bringup"),
            "launch",
            "elfin_controller.launch.py",
        )
        return [
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(controller_launch),
                launch_arguments={
                    "robot_model": model,
                    "robot_ip": LaunchConfiguration("robot_ip"),
                    "control_mode": LaunchConfiguration("control_mode"),
                    "servo_gain": LaunchConfiguration("servo_gain"),
                    "lookahead_time": LaunchConfiguration("lookahead_time"),
                    "state_stale_timeout_ms": LaunchConfiguration("state_stale_timeout_ms"),
                    "state_disconnect_timeout_ms": LaunchConfiguration("state_disconnect_timeout_ms"),
                    "position_command_epsilon": LaunchConfiguration("position_command_epsilon"),
                    "velocity_command_epsilon": LaunchConfiguration("velocity_command_epsilon"),
                    "command_log_throttle_ms": LaunchConfiguration("command_log_throttle_ms"),
                    "servo_restart_idle_ms": LaunchConfiguration("servo_restart_idle_ms"),
                    "enable_controller_validation": LaunchConfiguration("enable_controller_validation"),
                    "pushed_state_port": LaunchConfiguration("pushed_state_port"),
                    "pushed_state_socket_timeout_ms": LaunchConfiguration("pushed_state_socket_timeout_ms"),
                    "pushed_state_disconnect_timeout_ms": LaunchConfiguration("pushed_state_disconnect_timeout_ms"),
                    "status_publish_rate": LaunchConfiguration("status_publish_rate"),
                    "io_publish_rate": LaunchConfiguration("io_publish_rate"),
                    "update_rate": LaunchConfiguration("update_rate"),
                }.items(),
            ),
        ]

    if hardware_type == "ethercat":
        ethercat_launch = os.path.join(
            get_package_share_directory("elfin_robot_bringup"),
            "launch",
            "elfin_ros2_ethercat.launch.py",
        )
        return [
            IncludeLaunchDescription(
                PythonLaunchDescriptionSource(ethercat_launch)
            )
        ]

    if hardware_type == "fake":
        raise RuntimeError(
            "hardware_type 'fake' does not yet have a complete bringup path; "
            "use 'gazebo', 'controller', or 'ethercat'"
        )

    raise RuntimeError(
        f"Unsupported hardware_type '{hardware_type}'. "
        "Choose gazebo, controller, ethercat, or fake."
    )


def generate_launch_description():
    default_config = os.path.join(
        get_package_share_directory("elfin_robot_bringup"),
        "config",
        "elfin_control.yaml",
    )
    return LaunchDescription(
        [
            DeclareLaunchArgument(
                "config_file",
                default_value=default_config,
                description="YAML file containing unified Elfin bringup settings",
            ),
            OpaqueFunction(function=_load_config),
            DeclareLaunchArgument(
                "robot_model",
                default_value="E05",
                description="Robot model name",
            ),
            DeclareLaunchArgument(
                "hardware_type",
                default_value="gazebo",
                description="gazebo, controller, ethercat, or fake",
            ),
            DeclareLaunchArgument(
                "robot_ip",
                default_value="10.20.200.3",
                description="Robot controller IP for hardware_type=controller",
            ),
            DeclareLaunchArgument(
                "control_mode",
                default_value="position",
                description="position, velocity, or controller",
            ),
            DeclareLaunchArgument("servo_gain", default_value="8000"),
            DeclareLaunchArgument("lookahead_time", default_value="0.004"),
            DeclareLaunchArgument("state_stale_timeout_ms", default_value="100"),
            DeclareLaunchArgument("state_disconnect_timeout_ms", default_value="1000"),
            DeclareLaunchArgument("position_command_epsilon", default_value="1e-8"),
            DeclareLaunchArgument("velocity_command_epsilon", default_value="1e-8"),
            DeclareLaunchArgument("command_log_throttle_ms", default_value="100"),
            DeclareLaunchArgument("servo_restart_idle_ms", default_value="100"),
            DeclareLaunchArgument("enable_controller_validation", default_value="true"),
            DeclareLaunchArgument("pushed_state_port", default_value="10004"),
            DeclareLaunchArgument("pushed_state_socket_timeout_ms", default_value="100"),
            DeclareLaunchArgument("pushed_state_disconnect_timeout_ms", default_value="1000"),
            DeclareLaunchArgument("status_publish_rate", default_value="10.0"),
            DeclareLaunchArgument("io_publish_rate", default_value="5.0"),
            DeclareLaunchArgument("update_rate", default_value="1000"),
            OpaqueFunction(function=_configure),
        ]
    )
