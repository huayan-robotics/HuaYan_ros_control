#!/usr/bin/env python3
"""Start only the GUI when elfin_sdk_node is already running."""

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package="elfin_basic_api",
            executable="elfin_gui_new.py",
            name="elfin_gui",
            output="screen",
        ),
    ])
