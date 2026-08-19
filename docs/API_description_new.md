# Elfin ROS 2 API Reference

[中文](API_description_new_cn.md)

This document defines the controller SDK node, ROS 2 interfaces, units, GUI
mappings, and motion supervision. Build, launch, parameter,
model, and version instructions are provided in
[`README.md`](../README.md).

## 1. Units and Frames

- Standard ROS joint position and velocity use rad and rad/s.
- GUI joint values use degrees.
- `/elfin_sdk/realtime_state` uses m and m/s for TCP translation and rad for
  TCP orientation.
- SDK setting services use controller-native units: mm and degrees.
- X/Y/Z/RX/RY/RZ define the TCP pose relative to Base.
- Cartesian jog axes 0 through 5 map to X, Y, Z, RX, RY, and RZ.
- The default TCP name is `TCP`; the user frame is `Base`.

## 2. State Topics

### 2.1 `/joint_states`

Type: `sensor_msgs/msg/JointState`.

The hardware interface writes port 8893 feedback into ros2_control state
interfaces. `joint_state_broadcaster` publishes the standard topic consumed by
MoveIt, RViz, TF, and ROS applications. Position and velocity use rad and rad/s.

### 2.2 `/elfin_sdk/joint_states`

Type: `sensor_msgs/msg/JointState`.

The hardware interface publishes the same port 8893 joint feedback directly.
This diagnostic path bypasses `joint_state_broadcaster` and verifies parsing and
unit conversion. It is not the primary MoveIt, RViz, or TF input.

```text
8893
  |-- ros2_control state interfaces -> joint_state_broadcaster
  |                                      -> /joint_states
  `-- ElfinControllerHardware -> /elfin_sdk/joint_states
```

Both topics use the same message type because both represent joint names,
positions, velocities, and efforts. Use `/elfin_sdk/realtime_state` for the
additional target, TCP, force, and controller fields.

### 2.3 `/elfin_sdk/realtime_state`

Type: `elfin_robot_msgs/msg/ElfinRealtimeState`. Source: TCP 8893.

| Field | Description | Unit |
|---|---|---|
| `joint_position_target/actual` | Target/actual joint position | rad |
| `joint_velocity_target/actual` | Target/actual joint velocity | rad/s |
| `joint_torque_actual` | Actual joint torque | protocol unit |
| `tcp_position_target/actual` | Target/actual TCP pose | m, rad |
| `tcp_velocity_target/actual` | Target/actual TCP velocity | m/s, rad/s |
| `force_raw` | Raw force data | protocol unit |
| `force_calibrated` | Calibrated force data | protocol unit |
| `speed_scaling` | Active speed scaling | 0 to 1 |
| `controller_time_us` | Controller timestamp | us |
| `state_machine` | Controller state-machine value | enum |
| `force_control_state` | Force-control state | enum |

### 2.4 `/elfin_sdk/robot_status`

Type: `elfin_robot_msgs/msg/ElfinRobotStatus`.

| Field | Description |
|---|---|
| `sdk_connected` | TCP 10003 SDK connection |
| `controller_connected` | Controller connection |
| `electrified` | Robot power state |
| `controller_started` | Controller initialization state |
| `enabled` | Servo On state |
| `moving`, `in_position`, `blending_done` | Motion completion state |
| `error`, `error_code`, `error_axis` | Controller fault state |
| `braking`, `paused` | Brake and pause state |
| `freedrive`, `force_freedrive` | Free-drive states |
| `ros_control_active` | ROS motion-controller state |
| `emergency_stop`, `safeguard_stop` | Safety state fields |

Motion, enable, pause, in-position, brake, and error fields are decoded from
the TCP 10004 `StateAndError` object.

### 2.5 `/elfin_sdk/brake_state`

Type: `elfin_robot_msgs/msg/ElfinBrakeState`.

- `raw_state[6]`: raw TCP 10004 `BrakeState` values;
- `released[6]`: normalized release state for applications.

### 2.6 `/elfin_sdk/io_state`

Type: `elfin_robot_msgs/msg/ElfinIOState`. Source: TCP 10004.

Fields: `digital_inputs`, `digital_outputs`, `configurable_inputs`,
`configurable_outputs`, `analog_inputs`, `analog_output_modes`, and
`analog_outputs`.

### 2.7 `/elfin_sdk/end_io_state`

Type: `elfin_robot_msgs/msg/ElfinEndIOState`. Source: TCP 10004 `EndIO`.

Fields: four `digital_inputs`, four `digital_outputs`, four `buttons`,
`buttons_enabled`, and two `analog_inputs`. Array indices are channel numbers.

## 3. SDK Services

Custom services return `success` and `message`. HRIF failures preserve the
controller error code and error text in `message`.

### 3.1 Power, Initialization, and Servo

| Service | Type | Implementation |
|---|---|---|
| `/elfin_sdk/electrify` | `std_srvs/srv/Trigger` | `HRIF_Electrify`; waits for Power On |
| `/elfin_sdk/initialize_controller` | `std_srvs/srv/Trigger` | `HRIF_Connect2Controller`; requires power and Servo Off |
| `/elfin_sdk/set_enabled` | `std_srvs/srv/SetBool` | `HRIF_GrpEnable/GrpDisable`; disables ROS control first |
| `/elfin_sdk/blackout` | `std_srvs/srv/Trigger` | Stops ROS control, disables the group, then calls `HRIF_Blackout` |

### 3.2 Motion State and Control Ownership

| Service | Type | Implementation |
|---|---|---|
| `/elfin_sdk/reset` | `std_srvs/srv/Trigger` | `HRIF_GrpReset` |
| `/elfin_sdk/stop` | `std_srvs/srv/Trigger` | Stops active SDK motion and invokes group/safeguard stop logic |
| `/elfin_sdk/pause` | `std_srvs/srv/Trigger` | `HRIF_GrpInterrupt` |
| `/elfin_sdk/continue` | `std_srvs/srv/Trigger` | `HRIF_GrpContinue` |
| `/elfin_sdk/set_ros_control` | `std_srvs/srv/SetBool` | `controller_manager/SwitchController` |
| `/elfin_sdk/set_freedrive` | `std_srvs/srv/SetBool` | `HRIF_GrpOpen/CloseFreeDriver` |
| `/elfin_sdk/set_force_freedrive` | `std_srvs/srv/SetBool` | `HRIF_SetForceFreeDriveMode` |
| `/elfin_sdk/set_speed_ratio` | `SetFloat64` | `HRIF_SetOverride`, range 0.01 to 1.0 |
| `/elfin_sdk/set_override` | `SetFloat64` | Compatibility alias for `set_speed_ratio` |

The free-drive modes are mutually exclusive. Entering either mode deactivates
the ROS motion controller. Exiting free drive does not restore a previous
trajectory. Reactivate ROS control explicitly after the robot state is stable.

### 3.3 TCP and UCS

`SetPose.pose` uses `[X, Y, Z, RX, RY, RZ]` in mm and degrees.

| Service | Type | Implementation |
|---|---|---|
| `/elfin_sdk/get_tcp` | `GetPose` | `HRIF_ReadCurTCP`; current values without a name |
| `/elfin_sdk/set_tcp` | `SetPose` | `HRIF_SetTCP`; temporary current TCP |
| `/elfin_sdk/get_tcp_config` | `GetTcpConfig` | Active TCP name and controller values |
| `/elfin_sdk/configure_tcp` | `ConfigureTcp` | Configure, select, set default, and verify a named TCP |
| `/elfin_sdk/restore_default_tcp` | `Trigger` | Restore default and active TCP name `TCP` |
| `/elfin_sdk/set_tcp_by_name` | `SetString` | `HRIF_SetTCPByName` |
| `/elfin_sdk/set_ucs` | `SetPose` | `HRIF_SetUCS` |
| `/elfin_sdk/set_ucs_by_name` | `SetString` | `HRIF_SetUCSByName` |

`configure_tcp` calls `HRIF_ConfigTCP`, `HRIF_SetDftTCP`,
`HRIF_SetTCPByName`, and `HRIF_ReadTCPByName` in sequence. The service succeeds
only when all six readback values match the request. TCP write and selection
services require Servo Off.

### 3.4 Collision Level and Payload

| Service | Type | Implementation |
|---|---|---|
| `/elfin_sdk/get_collision_level` | `GetInt32` | `HRIF_GetCollideLevel` |
| `/elfin_sdk/set_collision_level` | `SetInt16` | `HRIF_SetCollideLevel`, range 0 to 5, Servo Off |
| `/elfin_sdk/get_payload` | `GetPayload` | Current mass, center of gravity, and maximum payload |
| `/elfin_sdk/set_payload` | `SetPayload` | `HRIF_SetPayload`, Servo Off |

Payload mass uses kg. Center of gravity uses `[X, Y, Z]` in mm. Mass shall be
non-negative and shall not exceed the controller maximum. `option` accepts 1
or 3.

### 3.5 Brake

| Service | Type | Implementation |
|---|---|---|
| `/elfin_sdk/set_brake` | `SetBrake` | `release=true`: `HRIF_OpenBrake`; `false`: `HRIF_CloseBrake` |

`axis` ranges from 0 through 5. The service requires Servo Off. The GUI also
requires stationary, fault-free, valid robot state. Brake release is a
maintenance operation and requires physical joint support. Error 40017 from
`CloseBrake` is returned unchanged; the node does not initiate Blackout,
safeguard, or controller FSM transitions.

### 3.6 I/O

| Service | Type | Implementation |
|---|---|---|
| `/elfin_sdk/set_digital_io` | `SetDigitalIO` | Cabinet DO, configurable output, or tool DO |
| `/elfin_sdk/set_analog_io` | `SetAnalogIO` | `HRIF_SetBoxAOVal` |

`SetDigitalIO.domain` accepts:

- `box_do`: `HRIF_SetBoxDO`;
- `box_co`: `HRIF_SetBoxCO`;
- `end_do`: `HRIF_SetEndDO`.

DI, configurable input, and tool DI are read-only. DO, configurable output,
and tool DO are writable. TCP 10004 readback confirms output state.

### 3.7 Jog and Target Motion

| Service | Type | Implementation |
|---|---|---|
| `/elfin_sdk/jog` | `Jog` | `HRIF_LongJogJ`, `HRIF_LongJogL`, `HRIF_LongMoveEvent` |
| `/elfin_sdk/move_target` | `MoveTarget` | `HRIF_WayPoint`; Z alignment uses `HRIF_MoveAlignToZ` |

`Jog` request values:

- `mode=0/1`: joint/Cartesian;
- `axis=0..5`: J1 through J6 or X/Y/Z/RX/RY/RZ;
- `direction=0/1`: negative/positive;
- `action=0/1/2`: STOP/START/KEEPALIVE.

`MoveTarget` request values:

- `mode=0`: six joint targets in degrees;
- `mode=1`: `[X,Y,Z,RX,RY,RZ]` in mm and degrees;
- `mode=2`: Home at six zero-degree joint targets;
- `mode=3`: TCP Z-axis alignment relative to Base;
- `action=0/1/2`: STOP/START/KEEPALIVE;
- `hold_required`: hold-to-run operation.

Cartesian target motion is solved and executed by controller
`HRIF_WayPoint`, outside MoveIt collision planning.

## 4. Hold-to-Run and Watchdog

```text
Button press       -> START -> LongJogJ/LongJogL or WayPoint
Button held        -> KEEPALIVE about every 200 ms -> LongMoveEvent
Release/leave/focus loss/page change -> STOP
No keepalive for 500 ms              -> SDK node watchdog stop
```

The GUI and SDK node provide independent stop paths. One SDK motion operation
is active at a time. Starting a new operation terminates the previous one.

## 5. GUI-to-API Mapping

| GUI function | Interfaces |
|---|---|
| Robot Startup | `electrify`, `initialize_controller`, `set_enabled`, `blackout` |
| Servo On/Off | `set_enabled`, `robot_status` |
| Clear Fault | `reset` |
| Stop | `stop` |
| Velocity Scaling | `set_speed_ratio` |
| Free Drive | `set_freedrive` |
| ROS Control | `set_ros_control` |
| Joint/Cartesian Jog | `jog` |
| Hold-to-run/Home/Z Align | `move_target` |
| TCP | `get_tcp_config`, `configure_tcp`, `restore_default_tcp` |
| Safety Level | `get/set_collision_level` |
| Payload | `get/set_payload` |
| Brake | `set_brake`, `brake_state` |
| Set I/O | `set_digital_io`, I/O state topics |

## 6. ros2_control and MoveIt

### 6.1 Control Modes

| `control_mode` | ROS controller | Port 8892 | MoveIt execution | Purpose |
|---|---|---|---|---|
| `position` | `elfin_arm_controller` | ServoJ | Supported | MoveIt and trajectories |
| `controller` | Motion controller inactive | No command | Monitor/plan | Pendant, SDK, free drive |

### 6.2 State and Trajectory Paths

```text
8893 -> ElfinControllerHardware::read()
     -> joint_state_broadcaster -> /joint_states
     -> robot_state_publisher -> /tf -> MoveIt / RViz

MoveIt -> /elfin_arm_controller/follow_joint_trajectory
       -> ros2_control -> ElfinControllerHardware::write()
       -> 8892 StartServo + PushServoJ
```

Gazebo replaces the port 8892/8893 hardware layer with
`gazebo_ros2_control`.

### 6.3 ServoJ Write Strategy

1. Hardware activation initializes commands from actual port 8893 positions.
2. The first changed trajectory command transmits `StartServo`, then
   `PushServoJ`.
3. A continuous trajectory transmits `PushServoJ` only for changed targets.
4. An unchanged target for `servo_restart_idle_ms` ends the Servo stream.
5. The next trajectory starts a new Servo stream with `StartServo`.

### 6.4 ROS Control Readiness

The motion controller is loaded inactive. Startup activation requires a valid
TCP 10004 state, Servo On, no fault, no pause, all brakes released, and free
drive inactive.

Servo On after startup does not activate ROS control. Call
`set_ros_control(true)` explicitly. Servo Off, fault, pause, braking, or TCP
10004 loss deactivates the motion controller. State recovery does not
reactivate it automatically.

## 7. State Streams and Recovery

### 7.1 TCP 8893

- `state_stale_timeout_ms`: mark state stale and block port 8892 writes while
  retaining the last valid state;
- `state_disconnect_timeout_ms`: reconnect after sustained frame loss.

After reconnect, command interfaces synchronize to actual joint positions
before command transmission resumes. Hardware activation validates the first
frame `cycle_time`: 1 ms requires 1000 Hz and 4 ms requires 250 Hz.

### 7.2 TCP 10004

The stream uses a 12-byte little-endian header: ASCII magic `LTBR`,
`totalSize`, and `dataSize`, followed by UTF-8 JSON. The invariant is
`totalSize = 12 + dataSize`. The parser handles fragmented frames, combined
frames, magic resynchronization, JSON errors, receive timeout, and reconnect.

Stream loss terminates SDK hold-to-run motion and deactivates ROS motion
control. `status_publish_rate` and `io_publish_rate` control topic publication
rates independently of the TCP 8893 control rate.

## 8. Interface Diagnostics

```bash
ros2 control list_hardware_components -v
ros2 control list_controllers
ros2 control list_hardware_interfaces

ros2 topic hz /joint_states
ros2 topic echo /elfin_sdk/robot_status
ros2 topic echo /elfin_sdk/realtime_state
ros2 service list | grep elfin_sdk
ros2 action list | grep follow_joint_trajectory
```

Expected position-mode controller state:

```text
joint_state_broadcaster  active
elfin_arm_controller     active
```

Expected controller-control state:

```text
joint_state_broadcaster  active
elfin_arm_controller     inactive
```

## 9. Source Locations

| File | Responsibility |
|---|---|
| `elfin_basic_api/scripts/elfin_gui_new.py` | New GUI and ROS bridge |
| `elfin_basic_api/scripts/elfin_gui.py` | Legacy GUI |
| `elfin_controller_driver/src/sdk_node.cpp` | SDK services, topics, and watchdog |
| `elfin_controller_driver/src/controller_hardware.cpp` | Port 8892/8893 hardware interface |
| `elfin_controller_driver/src/rt_info_client.cpp` | TCP 10004 frame and JSON parser |
| `elfin_robot_msgs/msg/` | State message definitions |
| `elfin_robot_msgs/srv/` | Service definitions |
