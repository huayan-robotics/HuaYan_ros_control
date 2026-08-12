# Elfin controller driver

This package is the controller-backed replacement for the legacy SOEM hardware
plugin. Port 8893 is consumed by a dedicated state thread and port 8892 is used
for `StartServo`, `PushServoJ`, and `SpeedJ` commands. ROS joint units are radians;
the controller boundary defaults to degrees.

The SDK node exposes:

```text
/elfin_sdk/set_enabled      std_srvs/srv/SetBool
/elfin_sdk/set_ros_control  std_srvs/srv/SetBool
/elfin_sdk/set_freedrive    std_srvs/srv/SetBool
/elfin_sdk/set_force_freedrive std_srvs/srv/SetBool
/elfin_sdk/stop             std_srvs/srv/Trigger
/elfin_sdk/reset            std_srvs/srv/Trigger
/elfin_sdk/pause            std_srvs/srv/Trigger
/elfin_sdk/continue         std_srvs/srv/Trigger
/elfin_sdk/set_override     elfin_robot_msgs/srv/SetFloat64
/elfin_sdk/set_tcp          elfin_robot_msgs/srv/SetPose
/elfin_sdk/configure_tcp    elfin_robot_msgs/srv/ConfigureTcp
/elfin_sdk/get_tcp_config   elfin_robot_msgs/srv/GetTcpConfig
/elfin_sdk/restore_default_tcp std_srvs/srv/Trigger
/elfin_sdk/set_ucs          elfin_robot_msgs/srv/SetPose
/elfin_sdk/set_tcp_by_name  elfin_robot_msgs/srv/SetString
/elfin_sdk/set_ucs_by_name  elfin_robot_msgs/srv/SetString
/elfin_sdk/set_digital_io   elfin_robot_msgs/srv/SetDigitalIO
/elfin_sdk/set_analog_io    elfin_robot_msgs/srv/SetAnalogIO
```

`set_ros_control(false)` stops the configured motion controller but leaves the
joint-state broadcaster and port 8893 state updates running. Entering freedrive
also stops the motion controller first. Exiting freedrive deliberately does not
restart an old trajectory; call `set_ros_control(true)` explicitly after the arm
is stable.

`set_freedrive` is the normal zero-force teaching mode and uses
`HRIF_GrpOpenFreeDriver`. `set_force_freedrive` is the distinct force-control
free-drive mode and uses `HRIF_SetForceFreeDriveMode`. The two modes are mutually
exclusive.

Published topics:

```text
/elfin_sdk/realtime_state  elfin_robot_msgs/msg/ElfinRealtimeState (8893)
/elfin_sdk/joint_states    sensor_msgs/msg/JointState             (8893)
/elfin_sdk/robot_status    elfin_robot_msgs/msg/ElfinRobotStatus  (10004 JSON stream)
/elfin_sdk/io_state        elfin_robot_msgs/msg/ElfinIOState       (10004 JSON stream)
/elfin_sdk/end_io_state    elfin_robot_msgs/msg/ElfinEndIOState    (10004 JSON stream)
```

Joint values and angular TCP values in ROS messages are SI units (rad, rad/s).
TCP translations and linear velocities are converted from mm to m. `SetPose`
requests intentionally use the SDK-native units: mm and degrees.

Common SDK command examples:

```bash
ros2 service call /elfin_sdk/set_tcp elfin_robot_msgs/srv/SetPose \
  "{pose: [0.0, 0.0, 150.0, 0.0, 0.0, 0.0]}"

# Persist, select, and verify a named TCP in the controller configuration.
ros2 service call /elfin_sdk/configure_tcp elfin_robot_msgs/srv/ConfigureTcp \
  "{name: ROS_TCP, pose: [0.0, 0.0, 150.0, 0.0, 0.0, 0.0]}"

ros2 service call /elfin_sdk/set_digital_io elfin_robot_msgs/srv/SetDigitalIO \
  "{domain: box_do, index: 0, value: true}"

ros2 service call /elfin_sdk/set_analog_io elfin_robot_msgs/srv/SetAnalogIO \
  "{index: 0, mode: 0, value: 5.0}"

ros2 service call /elfin_sdk/set_override elfin_robot_msgs/srv/SetFloat64 \
  "{data: 0.5}"
```

Examples:

```bash
ros2 launch elfin_robot_bringup elfin_controller.launch.py \
  robot_model:=E05 robot_ip:=10.20.200.3 control_mode:=controller

ros2 service call /elfin_sdk/set_enabled std_srvs/srv/SetBool '{data: true}'
ros2 service call /elfin_sdk/set_freedrive std_srvs/srv/SetBool '{data: true}'
ros2 service call /elfin_sdk/set_freedrive std_srvs/srv/SetBool '{data: false}'
ros2 service call /elfin_sdk/set_ros_control std_srvs/srv/SetBool '{data: true}'
```

The position/velocity motion controller is initially loaded inactive. If the
first 10004 status frame reports enabled, unpaused, fault-free, and all joint
brakes released, the driver activates it automatically after loading completes.
If the robot is not ready at startup, enabling it later does not automatically
activate ROS control; call `set_ros_control(true)` explicitly. Loss of readiness
or of the 10004 stream deactivates it and requires another explicit activation
request after recovery.

The 8892 `SpeedJ` string is implemented as
`SpeedJ,0,J1,...,J6,acceleration,runtime,;`. Confirm this exact command spelling
and field order against the controller firmware before testing velocity mode.
