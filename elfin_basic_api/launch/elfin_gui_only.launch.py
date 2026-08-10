#!/usr/bin/env python3
"""Start only the GUI when elfin_sdk_node is already running."""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument("robot_model", default_value="E05"),
        DeclareLaunchArgument("robot_ip", default_value="192.168.56.103"),
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
