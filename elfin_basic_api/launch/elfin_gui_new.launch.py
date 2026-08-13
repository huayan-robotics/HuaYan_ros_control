#!/usr/bin/env python3
"""Start controller bringup and the Elfin control panel."""

import os
import yaml

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    IncludeLaunchDescription,
    OpaqueFunction,
    SetLaunchConfiguration,
)
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def _load_update_rate(context):
    """Use the unified YAML value unless this launch received an override."""
    if "update_rate" in context.launch_configurations:
        return []
    config_file = LaunchConfiguration("config_file").perform(context)
    if not os.path.isfile(config_file):
        raise RuntimeError(f"Elfin launch configuration file not found: {config_file}")
    with open(config_file, "r", encoding="utf-8") as stream:
        document = yaml.safe_load(stream) or {}
    values = document.get("elfin_control", document)
    update_rate = values.get("update_rate", 1000)
    try:
        update_rate = int(update_rate)
    except (TypeError, ValueError) as exception:
        raise RuntimeError("update_rate in elfin_control.yaml must be 250 or 1000") from exception
    if update_rate not in {250, 1000}:
        raise RuntimeError("update_rate in elfin_control.yaml must be 250 or 1000")
    return [SetLaunchConfiguration("update_rate", str(update_rate))]


def generate_launch_description():
    bringup_share = get_package_share_directory("elfin_robot_bringup")
    bringup = os.path.join(
        bringup_share,
        "launch",
        "elfin_control.launch.py",
    )
    default_config = os.path.join(bringup_share, "config", "elfin_control.yaml")
    return LaunchDescription([
        DeclareLaunchArgument("config_file", default_value=default_config),
        OpaqueFunction(function=_load_update_rate),
        DeclareLaunchArgument("update_rate", default_value="1000"),
        DeclareLaunchArgument("robot_model", default_value="E05"),
        DeclareLaunchArgument("robot_ip", default_value="192.168.56.103"),
        DeclareLaunchArgument("control_mode", default_value="controller"),
        DeclareLaunchArgument("enable_controller_validation", default_value="true"),
        IncludeLaunchDescription(
            PythonLaunchDescriptionSource(bringup),
            launch_arguments={
                "robot_model": LaunchConfiguration("robot_model"),
                "hardware_type": "controller",
                "robot_ip": LaunchConfiguration("robot_ip"),
                "control_mode": LaunchConfiguration("control_mode"),
                "enable_controller_validation": LaunchConfiguration(
                    "enable_controller_validation"
                ),
                "config_file": LaunchConfiguration("config_file"),
                "update_rate": LaunchConfiguration("update_rate"),
            }.items(),
        ),
        Node(
            package="elfin_basic_api",
            executable="elfin_gui_new.py",
            name="elfin_gui",
            output="screen",
            parameters=[{
                "robot_model": LaunchConfiguration("robot_model"),
                "robot_ip": LaunchConfiguration("robot_ip"),
            }],
        ),
    ])
