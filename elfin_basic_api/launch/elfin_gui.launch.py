#!/usr/bin/env python3
"""Start controller bringup and the Elfin control panel."""

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    bringup = os.path.join(
        get_package_share_directory("elfin_robot_bringup"),
        "launch",
        "elfin_control.launch.py",
    )
    return LaunchDescription([
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
            }.items(),
        ),
        Node(
            package="elfin_basic_api",
            executable="elfin_gui.py",
            name="elfin_gui",
            output="screen",
        ),
    ])
