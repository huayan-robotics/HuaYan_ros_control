#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import rclpy
from rclpy.node import Node
from std_msgs.msg import String
from sensor_msgs.msg import JointState
from std_srvs.srv import SetBool
from rclpy.duration import Duration
from geometry_msgs.msg import PoseStamped,PoseArray, Pose
from ament_package.templates import get_environment_hook_template_path
from trajectory_msgs.msg import JointTrajectory, JointTrajectoryPoint
import threading
import time

class test(object):
    def __init__(self):
        rclpy.init(args=None)
        self.node = Node('test')
        self.publisher_joint = self.node.create_publisher(JointState, '/joint_goal', 1)
        self.publisher_cart = self.node.create_publisher(PoseStamped,'/cart_goal', 1)
        self.publisher_cart_goal = self.node.create_publisher(PoseArray,'/cart_path_goal',1)
        self.trajectory_cmd = self.node.create_publisher(JointTrajectory,'/elfin_arm_controller/joint_trajectory',1)

    def function_pub_joints(self):
        robot_state = JointState()
        robot_state.name = ['elfin_joint1','elfin_joint2','elfin_joint3',
            'elfin_joint4','elfin_joint5','elfin_joint6']
        robot_state.header.stamp = self.node.get_clock().now().to_msg()
        robot_state.position = [0.0,0.0,1.57,0.0,1.57,0.0]
        self.publisher_joint.publish(robot_state)
    
    def function_pub_joints1(self):
        robot_state = JointState()
        robot_state.name = ['elfin_joint1','elfin_joint2','elfin_joint3',
            'elfin_joint4','elfin_joint5','elfin_joint6']
        robot_state.header.stamp = self.node.get_clock().now().to_msg()
        robot_state.position = [-1.57,-1.57,-1.57,-1.57,-1.57,-1.57]
        self.publisher_joint.publish(robot_state)

    def function_pub_cart(self):
        robot_state = PoseStamped()
        robot_state.header.frame_id = 'elfin_base_link'
        robot_state.header.stamp = self.node.get_clock().now().to_msg()
        robot_state.pose.position.x = 0.52
        robot_state.pose.position.y = 0.0
        robot_state.pose.position.z = 0.565
        robot_state.pose.orientation.x = 0.0
        robot_state.pose.orientation.y = 1.0
        robot_state.pose.orientation.z = 0.0
        robot_state.pose.orientation.w = 0.0
        self.publisher_cart.publish(robot_state)
    
    def function_pub_cart_path(self):
        pa = PoseArray()
        ps1 = Pose()
        ps2 = Pose()
        ps3 = Pose()
        pa.header.frame_id = 'elfin_base_link'
        pa.header.stamp = self.node.get_clock().now().to_msg()

        # elfin3 0.325 0 0.305 0 1 0 0
        # elfin5 0.42 0 0.445 0 1 0 0
        # elfin5_l 0.495 0 0.52 0 1 0 0
        # elfin10 0.52 0 0.565 0 1 0 0
        # elfin10_l 0.7 0 0.685 0 1 0 0
        # elfin15 0.57 0 0.82 0 1 0 0

        ps1.position.x = 0.52
        ps1.position.y = 0.0
        ps1.position.z = 0.565
        ps1.orientation.x = 0.0
        ps1.orientation.y = 1.0
        ps1.orientation.z = 0.0
        ps1.orientation.w = 0.0
        ps2.position.x = 0.62
        ps2.position.y = 0.0
        ps2.position.z = 0.665
        ps2.orientation.x = 0.0
        ps2.orientation.y = 1.0
        ps2.orientation.z = 0.0
        ps2.orientation.w = 0.0
        ps3.position.x = 0.52
        ps3.position.y = 0.0
        ps3.position.z = 0.565
        ps3.orientation.x = 0.0
        ps3.orientation.y = 1.0
        ps3.orientation.z = 0.0
        ps3.orientation.w = 0.0
        pa.poses.append(ps1)
        pa.poses.append(ps2)
        pa.poses.append(ps3)
        self.publisher_cart_goal.publish(pa)
    
    def function_pub_trajectory(self):
        self.joint1_name='elfin_joint1'
        self.joint2_name='elfin_joint2'
        self.joint3_name='elfin_joint3'
        self.joint4_name='elfin_joint4'
        self.joint5_name='elfin_joint5'
        self.joint6_name='elfin_joint6'

        self.action_goal=JointTrajectory()
        self.action_goal.joint_names=[self.joint1_name, self.joint2_name,self.joint3_name,
                                                 self.joint4_name,self.joint5_name,self.joint6_name]
        self.action_goal.header.stamp = self.node.get_clock().now().to_msg()
        point_goal=JointTrajectoryPoint()
        point_goal.positions=[0.1,0.1,0.1,0.1,0.1,0.1]
        point_goal.velocities=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        point_goal.accelerations=[0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
        point_goal.time_from_start.sec = 1
        point_goal.time_from_start.nanosec = 0
        
        self.action_goal.points.append(point_goal)
        self.trajectory_cmd.publish(self.action_goal)
        rclpy.spin(self.node)


if __name__ == '__main__':
    pub_test = test()
    # pub_test.function_pub_cart()
    pub_test.function_pub_cart_path()
    time.sleep(5)
    rclpy.spin(pub_test.node)