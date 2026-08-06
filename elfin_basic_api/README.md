# Elfin Control Panel

[中文说明](README_cn.md)

`elfin_basic_api` provides the controller-backed Elfin control panel. The GUI
uses ROS 2 topics and services exposed by `elfin_sdk_node`; it does not load the
HRIF library or connect directly to controller ports.

## Requirements

- Ubuntu 22.04 and ROS 2 Humble
- `wxPython` (`sudo apt install python3-wxgtk4.0`)
- An Elfin controller reachable on ports 8892, 8893, 10003, and 10004 as needed
- A controller model alias matching `robot_model`, for example `E05`

Build and source the workspace:

```bash
cd ~/workspace/HuaYan_ros_control
colcon build --symlink-install
source /opt/ros/humble/setup.bash
source install/setup.bash
```

## Launch

Start controller bringup and the GUI together:

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

If `elfin_sdk_node` and controller bringup are already running, start only the
GUI:

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py
```

Controller validation is enabled by default. It may be disabled only for a
known development controller:

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103 \
  enable_controller_validation:=false
```

## Functions

- Live controller, servo, fault, motion, brake, and I/O status
- Servo On/Off, Clear Fault, software emergency stop, and speed scaling
- Normal Free Drive and ROS Control mode switching
- Joint and Cartesian hold-to-run jog
- Joint/Cartesian target, Home, and TCP Z-axis alignment
- Current TCP, payload, collision safety level, and maintenance brake pages
- DI/DO, CI/CO, EndDI/EndDO feedback from the controller

All motion buttons use hold-to-run behavior. Releasing the mouse button stops
the active SDK motion, and a watchdog stops motion if keepalive messages cease.
The Brake page is available only while the robot is Servo Off, stationary, and
fault-free. `Release Brake` is a maintenance operation; support the manipulator
against gravity before using it.

Free Drive uses `HRIF_GrpOpenFreeDriver`, not force-controlled free drive. Its
availability depends on controller configuration. Force Free Drive remains a
separate SDK service and may require a force sensor.

## Architecture

```text
elfin_gui.py (wxPython/rclpy)
        | ROS 2 services and topics
elfin_sdk_node (C++/HRIF)
        | TCP 10003 commands + TCP 10004 pushed state
Elfin controller
```

The complete interface and implementation design is documented in
[`elfin_gui.md`](elfin_gui.md).
