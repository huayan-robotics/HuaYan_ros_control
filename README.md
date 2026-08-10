Elfin Robot
======


Chinese version of the README -> please [click here](./README_cn.md)

Controller-backed driver, MoveIt/Gazebo startup, services, and topics:
[中文使用手册](./docs/controller_driver_guide_cn.md)


<p align="center">
  <img src="docs/images/elfin.png" />
</p>

This repository provides ROS2 support for the Elfin Robot. The recommend operating environment is on Ubuntu 22.04 with ROS humble. So far These packages haven't been tested in other environment.

### Installation

#### Ubuntu 22.04 + ROS Humble

**Install some important dependent software packages:**
```sh
$ sudo apt-get install ros-humble-joint-trajectory-controller
$ sudo apt-get install ros-humble-controller-manager
$ sudo apt-get install ros-humble-trajectory-msgs
$ sudo apt-get install ros-humble-gazebo-ros2-control*
$ sudo apt-get install ros-humble-joint-state-controller
$ sudo apt-get install ros-humble-position-controllers
```

**Install related software packages:**
```sh
$ sudo apt-get install build-essential libgtk-3-dev
$ sudo pip3 install wxpython
$ sudo pip3 install transforms3d
```

**Install or upgrade MoveIt!.** 

If you have installed MoveIt!, please make sure that it's been upgraded to the latest version.

Install/Upgrade MoveIt!:

```sh
$ sudo apt-get update
$ sudo apt-get install ros-humble-moveit
```


**Install this repository from Source**

First set up a catkin workspace (see [this tutorials](http://wiki.ros.org/catkin/Tutorials)).  
Then clone the repository into the src/ folder. It should look like /path/to/your/catkin_workspace/src/elfin_robot.  
Make sure to source the correct setup file according to your workspace hierarchy, then use catkin_make to compile.  

Assuming your catkin workspace folder is ~/catkin_ws, you should use the following commands:
```sh
$ cd ~/catkin_ws/src
$ git clone -b humble_ethercat https://github.com/huayan-robotics/elfin_robot_ros2.git
$ cd ..
$ colcon build
$ source install/setup.bash
```


---

### Usage with Gazebo Simulation

***There are launch files available to bringup a simulated robot - either Elfin3, Elfin5 or Elfin10.  
In the following the commands for Elfin3 are given. For Elfin5 or Elfin10, simply replace the prefix accordingly.***

Bring up the simulated robot in Gazebo and Start up RViz with a configuration including the MoveIt!:
```sh
$ ros2 launch elfin3_ros2_moveit2 elfin3.launch.py
```

Start up elfin basic api and "Elfin Control Panel" interface:
```sh
$ ros2 launch elfin3_ros2_moveit2 elfin3_basic_api.launch.py
```

The controller-backed Elfin Control Panel is intended for an Elfin controller,
not the legacy fake/MoveIt GUI simulation path.

> Tutorial about how to use MoveIt! RViz plugin: [docs/moveit_plugin_tutorial_english.md](docs/moveit_plugin_tutorial_english.md)  
Tips:
Every time you want to plan a trajectory, you should set the start state to current first.


---

###  Usage with real Hardware

***There are launch files available to bringup a real robot - either Elfin3, Elfin5 or Elfin10.  
In the following the commands for Elfin3 are given. For Elfin5 or Elfin10, simply replace the prefix accordingly.***

Put the file *elfin_drivers.yaml*, that you got from the vendor, into the folder elfin_robot_bringup/config/.Then copy the parameters in this file to the elfin_robot_bringup/config/elfin_arm_control.yaml

Connect Elfin to the computer with a LAN cable. Then confirm the ethernet interface name of the connection with `ifconfig`. The default ethernet name is eth0. If the ethernet name is not eth0, you should correct the following line in the file *elfin_robot_bringup/config/elfin_arm_control.yaml* 

```
elfin_ethernet_name: eth0
```

Bring up the hardware of Elfin. Before bringing up the hardware, you should setup Linux with PREEMPT_RT properly. There is a [tutorial](https://wiki.linuxfoundation.org/realtime/documentation/howto/applications/preemptrt_setup). There are two versions of elfin EtherCAT slaves. Please bring up the hardware accordingly.

```sh
$ sudo chrt 10 bash
$ ros2 launch elfin3_ros2_moveit2 elfin3_moveit.launch.py
```

Start up RViz with a configuration including the MoveIt! Motion Planning plugin:
```sh
$ sudo su
$ ros2 launch elfin3_ros2_moveit2 elfin3_moveit_rviz.launch.py
```
Start up elfin basic api:
```sh
$ sudo su
$ ros2 launch elfin3_ros2_moveit2 elfin3_basic_api.launch.py
```
Start up "Elfin Control Panel" interface:
```sh
$ sudo su
$ ros2 launch elfin_basic_api elfin_gui_new.launch.py robot_model:=E05 robot_ip:=192.168.56.103
```

Enable the servos of Elfin with "Elfin Control Panel" interface: if there is no "Warning", just press the "Servo On" button to enable the robot. If there is "Warning", press the "Clear Fault" button first and then press the "Servo On" button.

Tutorial about how to use MoveIt! RViz plugin: [docs/moveit_plugin_tutorial_english.md](docs/moveit_plugin_tutorial_english.md)  
Tips:
Every time you want to plan a trajectory, you should set the start state to current first.

Before turning the robot off, you should press the "Servo Off" button to disable the robot.

For more information about API, see [docs/API_description_english.md](docs/API_description_english.md)
