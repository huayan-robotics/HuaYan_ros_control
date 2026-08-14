# Elfin ROS 2 API 接口说明

[English](API_description.md)

本文档是控制器SDK节点和GUI的接口说明。驱动编译、真机与Gazebo启动、启动
参数和版本限制见
[`controller_driver_guide_cn.md`](controller_driver_guide_cn.md)。

## 1. 通信架构

```text
elfin_gui_new.py（wxPython + rclpy）
        │ ROS 2 services/topics
elfin_sdk_node（C++）
        ├── HRIF SDK / TCP 10003：普通指令
        └── TCP 10004：机器人与IO状态推送

MoveIt / ros2_control
        ├── TCP 8892：StartServo、PushServoJ、SpeedJ
        └── TCP 8893：高速关节、TCP、力和控制周期状态
```

GUI不加载HRIF动态库，也不直接连接控制器端口。`rclpy` executor运行在后台线程；
ROS回调使用 `wx.CallAfter()` 更新界面，服务调用全部使用 `call_async()`，避免阻塞GUI
主线程。

| 端口 | 所属组件 | 用途 |
|---|---|---|
| 8892 | `ElfinControllerHardware` | ROS运动命令 |
| 8893 | `ElfinControllerHardware` | 高速反馈和控制周期 |
| 10003 | `elfin_sdk_node` | HRIF使能、状态设置、IO、TCP、点动等接口 |
| 10004 | `elfin_sdk_node` | JSON机器人、抱闸、控制柜IO和末端IO状态 |

## 2. 单位与坐标约定

- ROS标准关节位置、速度使用rad、rad/s；GUI显示使用degree。
- `/elfin_sdk/realtime_state` 的TCP平移、速度使用m、m/s，姿态使用rad。
- SDK设置接口沿用控制器原生单位：X/Y/Z为mm，RX/RY/RZ为degree。
- X/Y/Z/RX/RY/RZ表示TCP相对Base的末端位姿，不是六个关节角。
- `LongJogL` 的axis 0～5对应X、Y、Z、RX、RY、RZ。
- 默认TCP名称约定为 `TCP`，用户坐标系为 `Base`。

## 3. 状态话题

### 3.1 `/joint_states`

类型：`sensor_msgs/msg/JointState`。由 `joint_state_broadcaster` 发布，是MoveIt、RViz和
TF使用的标准关节状态，位置和速度单位为rad、rad/s。数据来源仍是8893：硬件插件先将
解析结果写入 `ros2_control` 状态接口，再由 `joint_state_broadcaster` 发布。正常ROS
应用应订阅该话题。

### 3.2 `/elfin_sdk/joint_states`

类型同样为 `sensor_msgs/msg/JointState`，因为它表达的仍是关节名称、位置、速度和
力矩。该话题由硬件插件直接根据8893解析结果发布，用于绕过
`joint_state_broadcaster` 检查底层解析和单位转换，不作为MoveIt、RViz或TF的主要输入。

两个话题来自同一份8893反馈，正常情况下数值应一致，区别仅在发布路径和用途：

```text
8893
  ├── ros2_control状态接口 → joint_state_broadcaster → /joint_states（标准应用）
  └── ElfinControllerHardware直接发布 → /elfin_sdk/joint_states（底层诊断）
```

需要查看8893中超出标准关节状态的TCP、力、目标值和控制器状态时，应使用
`/elfin_sdk/realtime_state`，而不是 `/elfin_sdk/joint_states`。

### 3.3 `/elfin_sdk/realtime_state`

类型：`elfin_robot_msgs/msg/ElfinRealtimeState`，来源为8893。

| 字段 | 含义 | 单位 |
|---|---|---|
| `joint_position_target/actual` | 六轴目标/实际位置 | rad |
| `joint_velocity_target/actual` | 六轴目标/实际速度 | rad/s |
| `joint_torque_actual` | 实际关节力矩 | 协议单位 |
| `tcp_position_target/actual` | TCP目标/实际位姿 | m、rad |
| `tcp_velocity_target/actual` | TCP目标/实际速度 | m/s、rad/s |
| `force_raw` | 原始力数据 | 协议单位 |
| `force_calibrated` | 标定力数据 | 协议单位 |
| `speed_scaling` | 实际速度比例 | 0～1 |
| `controller_time_us` | 控制器时间 | us |
| `state_machine` | 控制器状态机值 | 枚举值 |
| `force_control_state` | 力控状态 | 枚举值 |

### 3.4 `/elfin_sdk/robot_status`

类型：`elfin_robot_msgs/msg/ElfinRobotStatus`。主要字段：

| 字段 | 含义 |
|---|---|
| `sdk_connected` | 10003 SDK连接状态 |
| `controller_connected` | 控制器连接状态 |
| `electrified` | 已上电 |
| `controller_started` | 控制器已初始化 |
| `enabled` | Servo On |
| `moving`、`in_position`、`blending_done` | 运动和到位状态 |
| `error`、`error_code`、`error_axis` | 故障及原始错误信息 |
| `braking`、`paused` | 制动和暂停状态 |
| `freedrive`、`force_freedrive` | 普通/力控自由拖动状态 |
| `ros_control_active` | ROS运动控制器状态 |
| `emergency_stop`、`safeguard_stop` | 安全状态；受10004可用字段限制 |

机器人运动、使能、暂停、到位、抱闸和错误主要来自10004 `StateAndError`。控制器协议
未提供的物理急停字段不能代替硬件安全回路判断。

### 3.5 `/elfin_sdk/brake_state`

类型：`elfin_robot_msgs/msg/ElfinBrakeState`。

- `raw_state[6]`：10004 `BrakeState`原始值；
- `released[6]`：供应用使用的归一化松闸状态。

虚拟控制器HR6.5.22a验证中，`OpenBrake`使所选轴原始值从0变为1。该值属于手动松闸
标志，不能独立代表控制器FSM状态。

### 3.6 `/elfin_sdk/io_state`

类型：`elfin_robot_msgs/msg/ElfinIOState`，来源为10004。

字段包括 `digital_inputs`、`digital_outputs`、`configurable_inputs`、
`configurable_outputs`、`analog_inputs`、`analog_output_modes`和 `analog_outputs`。

### 3.7 `/elfin_sdk/end_io_state`

类型：`elfin_robot_msgs/msg/ElfinEndIOState`，来源为10004 `EndIO`。

字段包括四路 `digital_inputs`、四路 `digital_outputs`、四个 `buttons`、
`buttons_enabled`及两路 `analog_inputs`。数组下标即通道号。

## 4. SDK服务

### 4.1 上电、初始化与使能

| 服务 | 类型 | 实现与约束 |
|---|---|---|
| `/elfin_sdk/electrify` | `std_srvs/srv/Trigger` | `HRIF_Electrify`；等待上电状态完成 |
| `/elfin_sdk/initialize_controller` | `std_srvs/srv/Trigger` | `HRIF_Connect2Controller`；要求已上电且Servo Off |
| `/elfin_sdk/set_enabled` | `std_srvs/srv/SetBool` | `HRIF_GrpEnable/GrpDisable`；去使能前停止ROS控制 |
| `/elfin_sdk/blackout` | `std_srvs/srv/Trigger` | 停止ROS控制、去使能并等待Disable后调用 `HRIF_Blackout` |


### 4.2 运动状态与控制权

| 服务 | 类型 | 实现与约束 |
|---|---|---|
| `/elfin_sdk/reset` | `std_srvs/srv/Trigger` | `HRIF_GrpReset`；必要时先退出本节点的软件安全停止 |
| `/elfin_sdk/stop` | `std_srvs/srv/Trigger` | 结束当前SDK运动并调用组停止/安全防护停止逻辑 |
| `/elfin_sdk/pause` | `std_srvs/srv/Trigger` | `HRIF_GrpInterrupt` |
| `/elfin_sdk/continue` | `std_srvs/srv/Trigger` | `HRIF_GrpContinue` |
| `/elfin_sdk/set_ros_control` | `std_srvs/srv/SetBool` | 调用 `controller_manager/SwitchController` |
| `/elfin_sdk/set_freedrive` | `std_srvs/srv/SetBool` | 普通拖动：`HRIF_GrpOpen/CloseFreeDriver` |
| `/elfin_sdk/set_force_freedrive` | `std_srvs/srv/SetBool` | 力控拖动：`HRIF_SetForceFreeDriveMode` |
| `/elfin_sdk/set_speed_ratio` | `SetFloat64` | `HRIF_SetOverride`，范围0.01～1.0 |
| `/elfin_sdk/set_override` | `SetFloat64` | `set_speed_ratio`的兼容别名 |

两种freedrive互斥。进入任一种前停止ROS运动控制器；退出freedrive不会自动恢复旧轨迹，
需要状态稳定后显式调用 `set_ros_control(true)`。

### 4.3 TCP与UCS

`SetPose.pose` 顺序统一为 `[X, Y, Z, RX, RY, RZ]`，单位mm和degree。

| 服务 | 类型 | 实现与语义 |
|---|---|---|
| `/elfin_sdk/get_tcp` | `GetPose` | `HRIF_ReadCurTCP`，读取当前TCP数值，不含名称 |
| `/elfin_sdk/set_tcp` | `SetPose` | `HRIF_SetTCP`，设置临时当前TCP |
| `/elfin_sdk/get_tcp_config` | `GetTcpConfig` | 读取节点活动TCP名称及控制器参数 |
| `/elfin_sdk/configure_tcp` | `ConfigureTcp` | 创建/更新具名TCP、设为默认和当前，再回读校验 |
| `/elfin_sdk/restore_default_tcp` | `Trigger` | 将默认、当前及节点活动TCP恢复为名称 `TCP` |
| `/elfin_sdk/set_tcp_by_name` | `SetString` | `HRIF_SetTCPByName` |
| `/elfin_sdk/set_ucs` | `SetPose` | `HRIF_SetUCS` |
| `/elfin_sdk/set_ucs_by_name` | `SetString` | `HRIF_SetUCSByName` |

`configure_tcp` 依次调用 `HRIF_ConfigTCP`、`HRIF_SetDftTCP`、
`HRIF_SetTCPByName`和 `HRIF_ReadTCPByName`。只有回读六个值均与请求一致才返回成功。
设置、选择和恢复TCP要求Servo Off。

### 4.4 碰撞等级与负载

| 服务 | 类型 | 实现与约束 |
|---|---|---|
| `/elfin_sdk/get_collision_level` | `GetInt32` | `HRIF_GetCollideLevel` |
| `/elfin_sdk/set_collision_level` | `SetInt16` | `HRIF_SetCollideLevel`；范围0～5，要求Servo Off |
| `/elfin_sdk/get_payload` | `GetPayload` | 读取质量、质心和最大负载 |
| `/elfin_sdk/set_payload` | `SetPayload` | `HRIF_SetPayload`；要求Servo Off |

Payload质量单位kg，质心为 `[X,Y,Z]` mm。质量必须非负且不超过控制器最大负载，
`option`当前只接受1或3。

### 4.5 抱闸

| 服务 | 类型 | 实现 |
|---|---|---|
| `/elfin_sdk/set_brake` | `SetBrake` | `release=true`调用 `HRIF_OpenBrake`，否则调用 `HRIF_CloseBrake` |

`axis`范围0～5。该服务要求Servo Off；GUI还要求机器人静止、无故障和状态有效。松闸
属于维护操作，必须先支撑对应关节。`CloseBrake`返回40017时原样返回，不自动调用
Blackout、安全光幕或其他FSM切换。

### 4.6 IO

| 服务 | 类型 | 实现 |
|---|---|---|
| `/elfin_sdk/set_digital_io` | `SetDigitalIO` | 设置DO、CO或EndDO |
| `/elfin_sdk/set_analog_io` | `SetAnalogIO` | `HRIF_SetBoxAOVal` |

`SetDigitalIO.domain`仅接受：

- `box_do`：`HRIF_SetBoxDO`；
- `box_co`：`HRIF_SetBoxCO`；
- `end_do`：`HRIF_SetEndDO`。

DI、CI和EndDI只能读取；DO、CO和EndDO可写。写请求成功后仍应等待10004状态回读确认。

### 4.7 Jog与目标运动

| 服务 | 类型 | 实现 |
|---|---|---|
| `/elfin_sdk/jog` | `Jog` | `HRIF_LongJogJ`、`HRIF_LongJogL`、`HRIF_LongMoveEvent` |
| `/elfin_sdk/move_target` | `MoveTarget` | `HRIF_WayPoint`；Z对齐结合 `HRIF_MoveAlignToZ` |

`Jog`：

- `mode=0/1`：关节/笛卡尔；
- `axis=0..5`：J1～J6或X/Y/Z/RX/RY/RZ；
- `direction=0/1`：负向/正向；
- `action=0/1/2`：STOP/START/KEEPALIVE。

`MoveTarget`：

- `mode=0`：六个关节目标，单位degree；
- `mode=1`：TCP目标 `[X,Y,Z,RX,RY,RZ]`，单位mm和degree；
- `mode=2`：Home，目标固定为六轴0 degree；
- `mode=3`：TCP相对Base的Z轴对齐；
- `action=0/1/2`：STOP/START/KEEPALIVE；
- `hold_required`：要求按住运行。

笛卡尔目标由控制器 `HRIF_WayPoint` 求解和执行，绕过MoveIt碰撞规划。

## 5. 长按运动与watchdog

```text
按下按钮 → START → LongJogJ/LongJogL 或 WayPoint
持续按住 → 约每200 ms KEEPALIVE → LongMoveEvent
松开/离开/失焦/切页 → STOP
超过500 ms无keepalive → SDK节点watchdog自动停止
```

GUI和SDK节点各有一层终止保护。每次只允许一个活动运动；开始新操作会先结束旧操作。
状态流失联、窗口失焦或鼠标释放事件都会结束keepalive。

## 6. GUI功能与接口对应

| GUI功能 | 使用接口 |
|---|---|
| Robot Startup | `electrify`、`initialize_controller`、`set_enabled`、`blackout` |
| Servo On/Off | `set_enabled` + `robot_status` |
| Clear Fault | `reset` |
| Stop | `stop` |
| Velocity Scaling | `set_speed_ratio` |
| Free Drive | `set_freedrive`，不是力控freedrive |
| ROS Control | `set_ros_control` |
| Joint/Cartesian Jog | `jog` |
| Hold-to-run/Home/Z Align | `move_target` |
| TCP | `get_tcp_config`、`configure_tcp`、`restore_default_tcp` |
| Safety Level | `get/set_collision_level` |
| Payload | `get/set_payload` |
| Brake | `set_brake` + `brake_state` |
| Set I/O | `set_digital_io` + IO状态话题 |


## 7. ros2_control与MoveIt链路

### 7.1 控制模式

| `control_mode` | ROS控制器 | 8892命令 | MoveIt执行 | 用途 |
|---|---|---|---|---|
| `position` | `elfin_arm_controller` | ServoJ | 支持 | MoveIt和轨迹控制 |
| `velocity` | `elfin_velocity_controller` | SpeedJ | 当前配置不支持 | 关节速度控制 |
| `controller` | 运动控制器inactive | 不发送 | 仅监视/规划 | 示教器、SDK和freedrive |

对应控制器配置为 `elfin_controller_position.yaml`、
`elfin_controller_velocity.yaml`和 `elfin_controller_state_only.yaml`。

### 7.2 状态和轨迹数据流

```text
8893
  → ElfinControllerHardware::read()
  → joint_state_broadcaster
  → /joint_states
  → robot_state_publisher → /tf
  → MoveIt / RViz

MoveIt
  → /elfin_arm_controller/follow_joint_trajectory
  → ros2_control
  → ElfinControllerHardware::write()
  → 8892 StartServo + PushServoJ
```

Gazebo中由 `gazebo_ros2_control` 替代8892/8893硬件层，其余MoveIt和状态链路保持一致。

### 7.3 ServoJ写入策略

位置模式不会在每个update周期重复发送不变目标：

1. 激活时以8893实际位置建立命令基准，不立即启动ServoJ；
2. 轨迹目标首次变化时先发送 `StartServo`，再发送 `PushServoJ`；
3. 同一段连续轨迹只在目标数值变化时发送 `PushServoJ`；
4. 目标停止变化达到 `servo_restart_idle_ms` 后结束本段Servo流；
5. 下一段轨迹再次从 `StartServo` 开始。

### 7.4 ROS控制权和就绪条件

真实运动控制器先加载为 `inactive`。启动时只有首帧10004状态满足以下条件才自动激活：

- 已收到有效状态；
- 机器人已使能、无故障、未暂停；
- 六轴制动均已释放；
- freedrive未开启。

如果启动时未就绪，之后Servo On不会自动取得ROS控制权，必须显式调用
`set_ros_control(true)`。去使能、故障、暂停、制动或10004状态流丢失会自动停用运动
控制器；状态恢复后同样不会自动重新激活。

释放ROS控制权时关闭8892连接。正常Ctrl+C、SIGINT、SIGTERM、硬件生命周期退出和析构
都会关闭8892、8893及状态连接；不应使用 `kill -9` 作为正常退出方式。

## 8. 状态流、超时和恢复

### 8.1 8893高速状态

驱动使用两级超时：

- `state_stale_timeout_ms`：无完整帧时标记状态陈旧并禁止8892写入，但保留最后有效状态；
- `state_disconnect_timeout_ms`：持续无帧达到阈值后关闭并重连8893。

重连后先把命令接口同步到实际关节位置，再恢复写入，防止机器人跳回旧目标。TCP启用
keepalive；普通接收超时不会丢弃已经收到的半帧，非法帧、连接关闭或reset会进入重连。

控制器激活时读取8893首个有效包的 `cycle_time`。1 ms只接受1000 Hz，4 ms只接受
250 Hz，配置不一致时硬件拒绝激活。

### 8.2 10004推送状态

10004采用12字节小端帧头：ASCII魔数 `LTBR`、`totalSize`、`dataSize`，随后为UTF-8
JSON，并满足 `totalSize = 12 + dataSize`。解析器处理TCP粘包、半包、魔数重同步、
JSON错误、接收超时和自动重连。

状态流断开时停止SDK长按运动并停用ROS运动控制器。机器人状态和IO按照
`status_publish_rate`、`io_publish_rate`节流发布，不等同于8893控制频率。

## 9. 推荐的接口操作顺序

### 9.1 从控制器控制切换到ROS

1. 确认机器人已使能、无报警且不在制动或暂停状态；
2. 确认8892、8893、10003和10004可连接；
3. 退出两种freedrive；
4. 调用 `set_ros_control(true)`；
5. 确认 `elfin_arm_controller` 为 `active`；
6. 再执行MoveIt轨迹。

### 9.2 freedrive

1. 调用 `set_freedrive(true)`，驱动先停止ROS运动控制器；
2. 8893、`/joint_states`、TF和RViz继续更新；
3. 调用 `set_freedrive(false)` 退出；
4. 确认当前位置和现场安全；
5. 需要运动时显式调用 `set_ros_control(true)`。

关闭freedrive和恢复ROS控制必须分开，避免隐式恢复进入freedrive前的旧轨迹。

## 10. 接口诊断

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

位置模式预期状态：

```text
joint_state_broadcaster  active
elfin_arm_controller     active
```

控制器控制/只读模式预期状态：

```text
joint_state_broadcaster  active
elfin_arm_controller     inactive
```

设置 `command_log_throttle_ms:=0` 可记录每条8892命令。日志中的 `target_deg` 是发送目标，
`actual_deg` 是同一时刻8893实际反馈；`socket write: success`只代表写入TCP成功，不代表
控制器已接受或执行。

## 11. 关键源码

| 文件 | 内容 |
|---|---|
| `../elfin_basic_api/scripts/elfin_gui_new.py` | 新版GUI、ROS bridge和按钮状态机 |
| `../elfin_basic_api/scripts/elfin_gui.py` | 原有GUI |
| `../elfin_controller_driver/src/sdk_node.cpp` | 服务、话题、HRIF调用和watchdog |
| `../elfin_controller_driver/src/controller_hardware.cpp` | 8892/8893 ros2_control硬件接口 |
| `../elfin_controller_driver/src/rt_info_client.cpp` | 10004帧和JSON解析 |
| `../elfin_robot_msgs/msg/` | 状态消息定义 |
| `../elfin_robot_msgs/srv/` | 自定义服务定义 |
