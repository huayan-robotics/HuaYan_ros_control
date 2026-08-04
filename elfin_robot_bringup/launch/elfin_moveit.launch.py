#!/usr/bin/env python3
"""MoveIt and RViz only; the selected hardware bringup owns ros2_control and TF."""

import os
import yaml

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, OpaqueFunction
from launch.substitutions import Command, FindExecutable, LaunchConfiguration
from launch_ros.actions import Node


def _load_yaml(package_name, relative_path):
    path = os.path.join(get_package_share_directory(package_name), relative_path)
    with open(path, "r", encoding="utf-8") as stream:
        return yaml.safe_load(stream)


def _load_text(package_name, relative_path):
    path = os.path.join(get_package_share_directory(package_name), relative_path)
    with open(path, "r", encoding="utf-8") as stream:
        return stream.read()


def _configure(context):
    model = LaunchConfiguration("robot_model").perform(context)
    use_sim_time = LaunchConfiguration("use_sim_time")
    gazebo_package = f"{model}_ros2_gazebo"
    moveit_package = f"{model}_ros2_moveit2"

    xacro_file = os.path.join(
        get_package_share_directory(gazebo_package), "urdf", f"{model}.urdf.xacro"
    )
    robot_description = {
        "robot_description": Command(
            [
                FindExecutable(name="xacro"),
                " ",
                xacro_file,
                " use_fake_hardware:=false",
                " use_real_hardware:=false",
            ]
        )
    }
    semantic = {
        "robot_description_semantic": _load_text(
            moveit_package, f"config/{model}.srdf"
        )
    }
    kinematics = {
        "robot_description_kinematics": _load_yaml(
            moveit_package, "config/kinematics.yaml"
        )
    }
    ompl = {
        "move_group": {
            "planning_plugin": "ompl_interface/OMPLPlanner",
            "request_adapters": (
                "default_planner_request_adapters/AddTimeOptimalParameterization "
                "default_planner_request_adapters/FixWorkspaceBounds "
                "default_planner_request_adapters/FixStartStateBounds "
                "default_planner_request_adapters/FixStartStateCollision "
                "default_planner_request_adapters/FixStartStatePathConstraints"
            ),
            "start_state_max_bounds_error": 0.1,
        }
    }
    ompl["move_group"].update(
        _load_yaml(moveit_package, "config/ompl_planning.yaml")
    )
    moveit_controllers = {
        "moveit_simple_controller_manager": _load_yaml(
            moveit_package, "config/elfin_controllers.yaml"
        ),
        "moveit_controller_manager": (
            "moveit_simple_controller_manager/MoveItSimpleControllerManager"
        ),
    }
    common_parameters = [
        robot_description,
        semantic,
        kinematics,
        ompl,
        moveit_controllers,
        {
            "use_sim_time": use_sim_time,
            "moveit_manage_controllers": False,
            "trajectory_execution.allowed_execution_duration_scaling": 1.2,
            "trajectory_execution.allowed_goal_duration_margin": 0.5,
            "trajectory_execution.allowed_start_tolerance": 0.01,
            "publish_planning_scene": True,
            "publish_geometry_updates": True,
            "publish_state_updates": True,
            "publish_transforms_updates": True,
        },
    ]
    rviz_config = os.path.join(
        get_package_share_directory(moveit_package), "launch", f"{model}_moveit2.rviz"
    )
    return [
        Node(
            package="moveit_ros_move_group",
            executable="move_group",
            output="screen",
            parameters=common_parameters,
        ),
        Node(
            package="rviz2",
            executable="rviz2",
            name="rviz2",
            output="screen",
            arguments=["-d", rviz_config],
            parameters=[robot_description, semantic, kinematics, ompl, {"use_sim_time": use_sim_time}],
        ),
    ]


def generate_launch_description():
    return LaunchDescription(
        [
            DeclareLaunchArgument("robot_model", default_value="elfin5"),
            DeclareLaunchArgument("use_sim_time", default_value="false"),
            OpaqueFunction(function=_configure),
        ]
    )
