#!/usr/bin/env python3
"""Repeatedly plan and execute conservative random joint goals with MoveIt."""

import math
import random
import time
from typing import Dict, List, Optional

import rclpy
from moveit_msgs.action import MoveGroup
from moveit_msgs.msg import Constraints, JointConstraint, MoveItErrorCodes
from rclpy.action import ActionClient
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
from sensor_msgs.msg import JointState


class RandomJointMoveItTest(Node):
    def __init__(self) -> None:
        super().__init__("moveit_random_joint_test")
        self.group_name = self.declare_parameter("group_name", "elfin_arm").value
        self.joint_names = list(self.declare_parameter(
            "joint_names",
            [f"elfin_joint{i}" for i in range(1, 7)],
        ).value)
        self.targets_per_cycle = int(self.declare_parameter("targets_per_cycle", 10).value)
        self.loop = bool(self.declare_parameter("loop", True).value)
        self.execute = bool(self.declare_parameter("execute", True).value)
        self.max_offset = float(self.declare_parameter("max_offset", 0.15).value)
        self.pause_seconds = float(self.declare_parameter("pause_seconds", 1.0).value)
        self.allowed_planning_time = float(
            self.declare_parameter("allowed_planning_time", 5.0).value)
        self.action_timeout = float(self.declare_parameter("action_timeout", 60.0).value)
        self.velocity_scaling = float(self.declare_parameter("velocity_scaling", 0.1).value)
        self.acceleration_scaling = float(
            self.declare_parameter("acceleration_scaling", 0.1).value)
        self.goal_tolerance = float(self.declare_parameter("goal_tolerance", 0.01).value)
        seed = int(self.declare_parameter("random_seed", -1).value)
        if seed >= 0:
            random.seed(seed)

        if len(self.joint_names) != 6:
            raise ValueError("joint_names must contain exactly six joints")
        if self.targets_per_cycle <= 0 or self.max_offset <= 0.0:
            raise ValueError("targets_per_cycle and max_offset must be positive")
        if not 0.0 < self.velocity_scaling <= 1.0:
            raise ValueError("velocity_scaling must be in (0, 1]")
        if not 0.0 < self.acceleration_scaling <= 1.0:
            raise ValueError("acceleration_scaling must be in (0, 1]")

        # These broad clamps are a final guard only. The conservative target
        # region is centered on the initial measured pose and defaults to
        # +/-0.15 rad per joint.
        self.lower_limits = list(self.declare_parameter(
            "joint_lower_limits", [-math.pi] * 6).value)
        self.upper_limits = list(self.declare_parameter(
            "joint_upper_limits", [math.pi] * 6).value)
        if len(self.lower_limits) != 6 or len(self.upper_limits) != 6:
            raise ValueError("joint limit arrays must contain six values")

        self.current_positions: Dict[str, float] = {}
        self.create_subscription(
            JointState, "/joint_states", self._joint_state_callback,
            qos_profile_sensor_data)
        self.move_group_client = ActionClient(self, MoveGroup, "/move_action")

    def _joint_state_callback(self, message: JointState) -> None:
        for name, position in zip(message.name, message.position):
            self.current_positions[name] = position

    def wait_for_current_state(self, timeout: float = 10.0) -> Optional[List[float]]:
        deadline = time.monotonic() + timeout
        while rclpy.ok() and time.monotonic() < deadline:
            rclpy.spin_once(self, timeout_sec=0.1)
            if all(name in self.current_positions for name in self.joint_names):
                return [self.current_positions[name] for name in self.joint_names]
        return None

    def random_target(self, anchor: List[float]) -> List[float]:
        return [
            min(upper, max(lower, center + random.uniform(-self.max_offset, self.max_offset)))
            for center, lower, upper in zip(anchor, self.lower_limits, self.upper_limits)
        ]

    def make_goal(self, target: List[float]) -> MoveGroup.Goal:
        goal = MoveGroup.Goal()
        goal.request.group_name = self.group_name
        goal.request.num_planning_attempts = 5
        goal.request.allowed_planning_time = self.allowed_planning_time
        goal.request.max_velocity_scaling_factor = self.velocity_scaling
        goal.request.max_acceleration_scaling_factor = self.acceleration_scaling
        goal.request.start_state.is_diff = True

        constraints = Constraints()
        constraints.name = "random_joint_goal"
        for name, position in zip(self.joint_names, target):
            constraint = JointConstraint()
            constraint.joint_name = name
            constraint.position = position
            constraint.tolerance_above = self.goal_tolerance
            constraint.tolerance_below = self.goal_tolerance
            constraint.weight = 1.0
            constraints.joint_constraints.append(constraint)
        goal.request.goal_constraints = [constraints]

        goal.planning_options.plan_only = not self.execute
        goal.planning_options.look_around = False
        goal.planning_options.replan = True
        goal.planning_options.replan_attempts = 2
        goal.planning_options.replan_delay = 0.2
        return goal

    def run_goal(self, index: int, target: List[float]) -> bool:
        formatted = ", ".join(f"{value:.4f}" for value in target)
        operation = "规划并执行" if self.execute else "仅规划"
        self.get_logger().info(f"目标 {index}: {operation}, joint rad=[{formatted}]")

        send_future = self.move_group_client.send_goal_async(self.make_goal(target))
        rclpy.spin_until_future_complete(self, send_future, timeout_sec=10.0)
        if not send_future.done() or send_future.result() is None:
            self.get_logger().error(f"目标 {index}: 发送MoveGroup goal超时")
            return False
        goal_handle = send_future.result()
        if not goal_handle.accepted:
            self.get_logger().error(f"目标 {index}: MoveGroup拒绝goal")
            return False

        result_future = goal_handle.get_result_async()
        rclpy.spin_until_future_complete(
            self, result_future, timeout_sec=self.action_timeout)
        if not result_future.done() or result_future.result() is None:
            self.get_logger().error(f"目标 {index}: 规划/执行超时，正在取消")
            goal_handle.cancel_goal_async()
            return False

        result = result_future.result().result
        if result.error_code.val != MoveItErrorCodes.SUCCESS:
            self.get_logger().error(
                f"目标 {index}: MoveIt失败，error_code={result.error_code.val}")
            return False
        self.get_logger().info(
            f"目标 {index}: 成功，planning_time={result.planning_time:.3f}s")
        return True

    def run(self) -> None:
        self.get_logger().info("等待 /move_action ...")
        if not self.move_group_client.wait_for_server(timeout_sec=15.0):
            raise RuntimeError("/move_action action server不可用")
        self.get_logger().info("等待完整 /joint_states ...")
        anchor = self.wait_for_current_state()
        if anchor is None:
            raise RuntimeError("10秒内没有收到六轴 /joint_states")

        self.get_logger().warn(
            "将执行随机真实运动；请确认工作空间无人、机械臂已使能且ROS控制器为active")
        cycle = 0
        while rclpy.ok():
            cycle += 1
            success_count = 0
            self.get_logger().info(
                f"开始第 {cycle} 轮，共 {self.targets_per_cycle} 个随机目标")
            for index in range(1, self.targets_per_cycle + 1):
                if not rclpy.ok():
                    return
                target = self.random_target(anchor)
                if self.run_goal(index, target):
                    success_count += 1
                end = time.monotonic() + self.pause_seconds
                while rclpy.ok() and time.monotonic() < end:
                    rclpy.spin_once(self, timeout_sec=min(0.1, end - time.monotonic()))
            self.get_logger().info(
                f"第 {cycle} 轮完成：{success_count}/{self.targets_per_cycle} 成功")
            if not self.loop:
                return


def main() -> None:
    rclpy.init()
    node = RandomJointMoveItTest()
    try:
        node.run()
    except (KeyboardInterrupt, RuntimeError, ValueError) as error:
        node.get_logger().error(str(error))
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == "__main__":
    main()
