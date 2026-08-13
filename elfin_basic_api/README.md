# Elfin Control Panel

[中文说明](README_cn.md)

`elfin_basic_api` provides an Elfin operator interface connected to the robot
controller. The GUI communicates with `elfin_sdk_node` only through ROS 2
topics and services. It neither loads the HRIF shared library directly nor
connects directly to any controller port.

## Requirements

- Ubuntu 22.04 and ROS 2 Humble
- `wxPython` (`sudo apt install python3-wxgtk4.0`)
- Access to controller ports 8892, 8893, 10003, and 10004 as required
- A `robot_model` value matching the controller model alias, such as `E05`

Build and source the workspace:

```bash
cd ~/workspace/HuaYan_ros_control
colcon build --symlink-install
source /opt/ros/humble/setup.bash
source install/setup.bash
```

## Launch

This package keeps both the legacy GUI and the new controller-backed GUI:

- Legacy GUI: `scripts/elfin_gui.py`, using the original Basic API, MoveIt, TF,
  and trajectory-controller interfaces.
- New GUI: `scripts/elfin_gui_new.py`, using controller SDK topics and services
  provided by `elfin_sdk_node`.

### New GUI: complete bringup

Start controller bringup, `elfin_sdk_node`, and the new GUI together:

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

The complete GUI launch reads `update_rate` from the unified
`elfin_robot_bringup/config/elfin_control.yaml` file. Use 1000 Hz for a normal
1 ms controller and 250 Hz for a controller version whose update package name
contains `4ms`. Only this value needs to be changed for normal use:

```yaml
elfin_control:
  update_rate: 1000   # 1 ms version; use 250 for a 4 ms version
```

The value can also be overridden for one GUI launch:

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103 \
  update_rate:=250
```

After connecting to port 8893, the driver verifies the actual `cycle_time`:
1 ms accepts only 1000 Hz and 4 ms accepts only 250 Hz. A mismatch reports
`Control frequency mismatch` and shuts down the complete launch. This parameter
controls the controller_manager and 8892/8893 real-time loop, not GUI refresh.

### New GUI: GUI only

If controller bringup and `elfin_sdk_node` are already running in another
terminal, start only the new GUI:

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py
```

GUI-only mode does not start controller_manager and therefore has no
`update_rate` argument. Its control frequency is determined by the controller
bringup that is already running.

The new script can also be run directly:

```bash
ros2 run elfin_basic_api elfin_gui_new.py
```

### Legacy GUI

First start the robot driver, `elfin_basic_api_node`, MoveIt/trajectory
controller, and the related I/O services with the original package workflow.
Then run:

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py
```

This is the original real-robot GUI launch file. It sets
`use_fake_robot:=false` and `use_sim_time:=false`. The legacy script can also
be run directly:

```bash
ros2 run elfin_basic_api elfin_gui.py
```

The legacy GUI depends on interfaces including `/joint_teleop`,
`/cart_teleop`, `/stop_teleop`, `/home_teleop`, `/read_di`, `/read_do`,
`/write_do`, `/joint_states`, TF, and
`elfin_arm_controller/follow_joint_trajectory`. The window can open without
these backend nodes, but its controls and status will not work correctly.

`elfin_basic_api.launch.py` is a historical backend-only entry point. It starts
the legacy `elfin_basic_api_node` with model resources currently hard-coded for
Elfin10; it does not start the driver, MoveIt, either GUI, or simulation and is
not a replacement for the new complete GUI launch.

### Legacy fake GUI

The original fake launch is retained and starts the legacy `elfin_gui.py`:

```bash
ros2 launch elfin_basic_api fake_elfin_gui.launch.py
```

This launch only passes `use_fake_robot` and `use_sim_time` to the legacy GUI.
It does not start Gazebo, MoveIt, the legacy Basic API, or I/O services; start
those dependencies first with the original simulation workflow.

### Controller validation

Model and controller-version validation are enabled by default. Disable them
temporarily only when using a controller that is explicitly intended for
development:

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103 \
  enable_controller_validation:=false
```

## Features

- Live controller, servo, fault, motion, brake, and I/O status
- Servo On/Off, Clear Fault, software emergency stop, and velocity scaling
- Normal Free Drive and ROS Control mode switching
- Hold-to-run joint and Cartesian jogging
- Joint/Cartesian targets, Home, and TCP Z-axis alignment
- Current TCP, payload, collision safety level, and maintenance brake pages
- Read-only DI, CI, and EndDI feedback
- Clickable DO, CO, and EndDO control, with output colors ultimately confirmed
  by controller feedback

All motion buttons use hold-to-run behavior. Releasing the mouse button sends a
stop request. The driver watchdog also stops motion if keepalive messages are
interrupted. The Brake page permits operation only when the robot is Servo Off,
stationary, and fault-free. Brake release is a maintenance operation. Support
the manipulator before releasing a brake to prevent a joint from dropping under
gravity.

The Free Drive control calls the normal free-drive interface
`HRIF_GrpOpenFreeDriver`; it is not force-controlled free drive. Availability
depends on controller configuration. Force-controlled free drive is a separate
SDK service and may require a force sensor.

## Communication Architecture

```text
elfin_gui_new.py (wxPython/rclpy)
        | ROS 2 services/topics
elfin_sdk_node (C++/HRIF)
        | TCP 10003 commands + TCP 10004 pushed state
Elfin controller
```

For the complete interface and implementation design, see
[`elfin_gui.md`](elfin_gui.md).
