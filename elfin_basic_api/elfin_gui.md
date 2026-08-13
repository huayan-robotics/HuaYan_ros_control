# Elfin GUI 实现与通信说明


## 1. 总体架构

```text
wxPython GUI（主线程）
        │ wx.CallAfter / asynchronous result
GuiRosBridge（rclpy executor 后台线程）
        │ ROS 2 topic / asynchronous service
        ▼
elfin_sdk_node（C++/rclcpp）
        │
        ├─ 10003/TCP：HRIF SDK 请求、控制和设置
        ├─ 10004/TCP：控制器主动推送状态、IO、抱闸等数据
        └─ controller_manager：切换 ROS Control 控制器

ros2_control hardware
        ├─ 8893/TCP：高速状态数据
        └─ 8892/TCP：ROS Control 命令通道
```

GUI 不直接连接控制器端口。它只通过 ROS 2 与 `elfin_sdk_node` 通信；SDK 节点负责调用 HRIF SDK。ROS Control 被启用后，轨迹命令由 ros2_control 硬件接口使用 8892/8893 通道执行。

### 1.1 控制器端口分工

| 端口 | 使用方 | 用途 |
|---|---|---|
| 10003 | `elfin_sdk_node` | HRIF SDK 控制、设置与查询 |
| 10004 | `elfin_sdk_node` | 控制器状态和 IO 主动推送 |
| 8893 | ros2_control hardware | 高速读取机器人状态 |
| 8892 | ros2_control hardware | ROS Control 命令连接；仅相关控制模式开放时可用 |

## 2. GUI 线程与 ROS 通信

- wxPython 界面在主线程运行 `wx.App.MainLoop()`。
- `rclpy` executor 在后台线程运行，避免 ROS 回调阻塞窗口。
- ROS 订阅回调不直接修改 wx 控件，而是用 `wx.CallAfter()` 把更新交回 GUI 主线程。
- 所有 service 都使用 `call_async()`，不在 GUI 主线程等待同步结果。
- GUI 对服务调用设置超时；超时、服务不可用和 SDK 错误均在界面上反馈。
- 状态超过允许时间未更新时，界面将其视为失联，停止运动 keepalive，并禁用依赖实时状态的操作。

## 3. ROS 2 接口

### 3.1 GUI 订阅的话题

| Topic | 消息 | 用途 |
|---|---|---|
| `/elfin_sdk/robot_status` | `ElfinRobotStatus` | SDK/控制器连接、使能、故障、运动、急停、防护停、Free Drive、ROS Control 等状态 |
| `/elfin_sdk/realtime_state` | `ElfinRealtimeState` | 实际关节角、TCP 位姿、速度缩放等实时值 |
| `/elfin_sdk/brake_state` | `ElfinBrakeState` | 六轴抱闸原始值和松闸状态 |
| `/elfin_sdk/io_state` | `ElfinIOState` | 控制柜 DI/DO、CI/CO 状态 |
| `/elfin_sdk/end_io_state` | `ElfinEndIOState` | 末端 EndDI/EndDO 状态 |

### 3.2 GUI 调用的服务

| Service | 类型 | SDK/内部实现 | GUI 功能 |
|---|---|---|---|
| `/elfin_sdk/set_enabled` | `std_srvs/SetBool` | `HRIF_GrpEnable` / `HRIF_GrpDisable` | Servo On/Off |
| `/elfin_sdk/electrify` | `std_srvs/Trigger` | `HRIF_Electrify` | Power On |
| `/elfin_sdk/initialize_controller` | `std_srvs/Trigger` | `HRIF_Connect2Controller` | Initialize Controller |
| `/elfin_sdk/blackout` | `std_srvs/Trigger` | `HRIF_Blackout` | Power Off |
| `/elfin_sdk/reset` | `std_srvs/Trigger` | `HRIF_GrpReset`，必要时退出软件安全停止 | Clear Fault |
| `/elfin_sdk/stop` | `std_srvs/Trigger` | 停止当前 SDK 运动并调用安全防护停止 | Stop |
| `/elfin_sdk/set_freedrive` | `std_srvs/SetBool` | `HRIF_GrpOpenFreeDriver` / `HRIF_GrpCloseFreeDriver` | 普通 Free Drive |
| `/elfin_sdk/set_ros_control` | `std_srvs/SetBool` | `controller_manager/SwitchController` | ROS Control 开关 |
| `/elfin_sdk/set_speed_ratio` | `SetFloat64` | `HRIF_SetOverride` | Velocity Scaling |
| `/elfin_sdk/get_tcp` | `GetPose` | `HRIF_ReadCurTCP` | 兼容接口：读取当前 TCP 参数，不包含名称 |
| `/elfin_sdk/get_tcp_config` | `GetTcpConfig` | `HRIF_ReadTCPByName` | 读取节点当前使用的 TCP 名称和参数 |
| `/elfin_sdk/set_tcp` | `SetPose` | `HRIF_SetTCP` | 兼容接口：设置临时当前 TCP |
| `/elfin_sdk/configure_tcp` | `ConfigureTcp` | `HRIF_ConfigTCP` + `HRIF_SetDftTCP` + `HRIF_SetTCPByName` + `HRIF_ReadTCPByName` | 写入具名 TCP，设为控制器默认和当前 TCP，并回读校验 |
| `/elfin_sdk/restore_default_tcp` | `Trigger` | `HRIF_SetDftTCP("TCP")` + `HRIF_SetTCPByName("TCP")` | 恢复约定的原始默认 TCP |
| `/elfin_sdk/get_collision_level` | `GetInt32` | `HRIF_GetCollideLevel` | 读取 Safety Level |
| `/elfin_sdk/set_collision_level` | `SetInt16` | `HRIF_SetCollideLevel` | 设置 Safety Level |
| `/elfin_sdk/get_payload` | `GetPayload` | `HRIF_ReadPayload`、`HRIF_ReadMaxPayload` | 读取负载 |
| `/elfin_sdk/set_payload` | `SetPayload` | `HRIF_SetPayload` | 设置负载和质心 |
| `/elfin_sdk/set_brake` | `SetBrake` | `HRIF_OpenBrake` / `HRIF_CloseBrake` | 维护松闸 |
| `/elfin_sdk/set_digital_io` | `SetDigitalIO` | `HRIF_SetBoxDO/SetBoxCO/SetEndDO` | 设置输出 IO |
| `/elfin_sdk/jog` | `Jog` | `HRIF_LongJogJ/LongJogL/LongMoveEvent` | 关节和笛卡尔点动 |
| `/elfin_sdk/move_target` | `MoveTarget` | `HRIF_WayPoint`；Z 对齐时先计算对齐目标 | Home、目标运动、Z-axis Alignment |

SDK 节点还保留 Pause、Continue、Force Free Drive 等接口，但当前 GUI 没有调用它们。GUI 的 Free Drive 是普通拖动，不是力控拖动。

## 4. 单位与坐标约定

- GUI 中 J1～J6 使用 degree。
- GUI 中 TCP 的 X/Y/Z 使用 mm，RX/RY/RZ 使用 degree。
- ROS 标准状态中关节角和姿态使用 rad，TCP 平移使用 m；GUI 显示前完成单位转换。
- SDK 8893 原始字段按控制器协议使用 degree 和 mm，硬件层转换为 ROS 标准单位。
- X/Y/Z/RX/RY/RZ 表示末端 TCP 相对 Base 坐标系的位姿，不是六个关节角。
- `HRIF_LongJogL` 的 axis 0～5 分别代表 X、Y、Z、RX、RY、RZ。
- SDK 节点启动时约定并选中原始默认 TCP 名称 `TCP`；用户坐标系固定为 `Base`。
- 保存新 TCP 后，笛卡尔目标和 Z 轴对齐使用节点记录的活动 TCP 名称，不再硬编码 `TCP`。

## 5. 各功能实现

### 5.0 Robot Startup

GUI启动时自动打开英文 `Robot Startup` 弹窗；点击顶部综合机器人状态也可再次打开。状态来自SDK真实反馈：`HRIF_ReadRobotState`提供上电状态，`HRIF_IsControllerStarted`提供初始化状态，10004提供使能状态。主按钮按当前状态显示 `Power On`、`Initialize Controller`、`Servo On`或`Servo Off`。`Power Off`可在使能状态直接调用且不做二次确认；驱动内部会先退出ROS Control、执行Servo Off并等待控制器进入`RobotDisable`，然后才调用`HRIF_Blackout`。普通GUI不暴露会关闭整个控制器操作系统的`HRIF_ShutdownRobot`。

顶部状态颜色为：`Power Off`和`Not Initialized`红色，`Servo Off`橙色，`Servo On`绿色。

### 5.1 顶部状态栏

状态栏显示机型、IP、SDK 连接、Servo 和 Fault。Servo/Fault 由 `/elfin_sdk/robot_status` 的实际值更新，不以按钮点击结果代替控制器反馈。实时状态过期时，相关控件会进入不可用状态。

### 5.2 Servo On / Servo Off

调用 `/elfin_sdk/set_enabled`。`true` 对应 `HRIF_GrpEnable`，`false` 对应 `HRIF_GrpDisable`。最终显示以控制器推送的 enabled 状态为准。

### 5.3 Clear Fault

调用 `/elfin_sdk/reset`。节点调用 `HRIF_GrpReset`；如果控制器处于本节点设置的软件安全停止状态，则先退出该状态再重试复位。不会自动解除物理急停或外部安全回路。

### 5.4 Stop

Stop 会先结束节点记录的 Jog、Home、目标运动或 Z 对齐，再进入 SDK 的安全防护停止。它属于软件停止功能，不等同于硬件急停按钮，也不能替代安全回路。

### 5.5 Velocity Scaling

滑块调用 `/elfin_sdk/set_speed_ratio`，节点使用 `HRIF_SetOverride` 设置控制器速度比例。GUI 对连续拖动产生的更新进行合并，避免大量同步请求阻塞界面。

### 5.6 Free Drive

调用 `/elfin_sdk/set_freedrive`，内部使用 `HRIF_GrpOpenFreeDriver/HRIF_GrpCloseFreeDriver`。这是普通自由拖动，不调用力控接口。控制器必须满足其自身的使能、模式和安全条件；失败时显示 SDK 原始错误。

### 5.7 ROS Control

调用 `/elfin_sdk/set_ros_control`，由 SDK 节点向 `controller_manager` 请求切换控制器。启用后，ros2_control 硬件接口使用 8892 发送命令、8893读取状态。8892 未监听或已被残留连接占用时，ROS Control 无法启动，但不影响 10003/10004 SDK 功能。

### 5.8 关节点动 J1～J6

按下 `-` 或 `+` 时向 `/elfin_sdk/jog` 发送 START，节点调用 `HRIF_LongJogJ`。按住期间 GUI 周期发送 KEEPALIVE，节点调用 `HRIF_LongMoveEvent`。松开、鼠标离开、切换功能、失去窗口焦点或状态失联时发送 STOP。

每次只能存在一个活动运动。开始新操作前会结束旧操作，避免一次点动后其他按钮失效。

### 5.9 笛卡尔点动 X/Y/Z/RX/RY/RZ

操作方式与关节点动相同，但节点调用 `HRIF_LongJogL`。axis 0～5 是 TCP 的六个笛卡尔分量，并非关节编号。

### 5.10 Hold-to-run 目标运动

用户可输入六个目标关节角，或输入 TCP 的 XYZRXRYRZ，然后按住 Hold-to-run 才开始运动。关节目标和笛卡尔目标均通过 `/elfin_sdk/move_target` 处理，SDK 节点内部使用 `HRIF_WayPoint`。松开按钮立即请求停止，不会把界面永久留在目标运动模式。

“Sync actual”把当前实际值复制到输入框，不会命令机器人运动。

### 5.11 Home

Home 的目标固定为六个关节角 `[0, 0, 0, 0, 0, 0]` degree。只有按住 Home 时才执行，松开即停止。Home 与普通目标运动共用 `/elfin_sdk/move_target` 和同一套 keepalive/watchdog 管理。

### 5.12 Z-axis Alignment

按住按钮后，节点调用 `HRIF_MoveAlignToZ`，由控制器基于当前 `TCP` 和 `Base` 计算目标关节角，再通过 `HRIF_WayPoint` 执行；松开时停止。GUI 会先确认 10004 状态出现实际运动，再以 `moving=false`、`in_position=true`、`blending_done=true` 判定到位，自动退出对齐模式并显示 `Z Aligned`。驱动同时清除该目标的 watchdog 状态，避免到位后再次发送停止。若控制器拒绝目标、逆解失败或状态不允许，GUI 显示 SDK 返回错误。

### 5.13 TCP

TCP 页面用于配置控制器数据库中的具名 TCP，其位姿为相对法兰中心的偏移：X/Y/Z 为 mm，RX/RY/RZ 为 degree。名称由用户输入。

- 页面打开时用 `/elfin_sdk/get_tcp_config` 读取活动 TCP 名称及控制器中的实际参数。
- 保存时调用 `/elfin_sdk/configure_tcp`：先用 `HRIF_ConfigTCP` 创建或更新具名 TCP，再用 `HRIF_SetDftTCP` 和 `HRIF_SetTCPByName` 将其设为控制器默认及当前 TCP，最后用 `HRIF_ReadTCPByName` 回读校验。
- 保存成功后，`LongJogL` 使用控制器当前 TCP，`WayPoint` 和 `MoveAlignToZ` 显式使用同一个活动 TCP 名称。
- `Restore Default` 调用 `/elfin_sdk/restore_default_tcp`，将控制器默认、当前及节点活动 TCP 都恢复为约定名称 `TCP`；不会删除用户创建的其他 TCP。
- 只有名称设置成功且六个回读值与请求值一致时，GUI 才显示保存成功。
- 仅 Servo Off、机器人静止且状态有效时允许写入。

### 5.14 Safety Level

页面通过 `/elfin_sdk/get_collision_level` 读取实际碰撞等级，而不是使用 GUI 默认值。保存时调用 `/elfin_sdk/set_collision_level`。只有 Servo Off、机器人静止且状态有效时允许修改。

### 5.15 Payload

页面读取当前负载质量、质心和最大负载；保存时调用 `HRIF_SetPayload`。质量使用 kg，质心 X/Y/Z 使用 mm。节点校验数据和控制器最大负载；只有 Servo Off、机器人静止且状态有效时允许修改。

### 5.16 Brake

Brake 是维护功能：

- 仅 Servo Off、机器人静止、无故障且状态有效时允许操作。
- 当前反馈为抱闸时，点击一次调用 `HRIF_OpenBrake`；当前反馈为松闸时，再点击一次调用 `HRIF_CloseBrake`。按钮状态始终以 10004 实际反馈为准，而不是本地点击次数。
- `/elfin_sdk/brake_state` 显示控制器 10004 推送的六轴原始状态，而不是用按钮状态模拟。
- `CloseBrake` 返回 40017 时原样显示错误；程序不会自动调用 Blackout、安全光幕或其他 FSM 切换。

虚拟控制器测试中，OpenBrake 可使对应轴原始值从 0 变为 1，但 CloseBrake 可能因“不在 RobotBraking state”被拒绝。因此该页面不能替代控制器规定的现场抱闸维护流程。

### 5.17 Set I/O

输入和输出采用不同逻辑：

- DI、CI、EndDI 是只读输入。灰色表示没有输入，亮色表示有输入，白色表示暂无有效反馈。
- DO、CO、EndDO 是可控制输出。点击后调用 `/elfin_sdk/set_digital_io` 切换目标值。
- 发出请求时按钮短暂禁用；最终颜色以 `/elfin_sdk/io_state` 或 `/elfin_sdk/end_io_state` 的控制器反馈为准。
- 调用失败会恢复原显示并给出错误，不把本地点击结果当成真实 IO。
- GUI 不显示 AI/AO。

控制柜 IO 来自 10004 推送。已用 DO0 完成闭环检查：写 1 后反馈变为 1，再写 0 后反馈恢复为 0。

## 6. 运动 keepalive 与 watchdog

长按运动不是“一次调用后永久运行”。完整链路如下：

```text
鼠标按下 → START → HRIF_LongJogJ/LongJogL 或 HRIF_WayPoint
按住期间 → 约每 200 ms KEEPALIVE → HRIF_LongMoveEvent
鼠标松开/离开/失焦/切页 → STOP → SDK 停止函数
超过 500 ms 未收到 keepalive → 驱动 watchdog 自动停止
```

GUI 和驱动各有一层保护。即使 GUI 的鼠标释放事件丢失，驱动 watchdog 也会终止节点记录的活动运动。控制器处于 Moving、Stopping 或 LongJogMoving 的短暂状态时，GUI 在仍按住按钮的前提下有限重试；松开后不再重试。

## 7. 新旧 GUI 与启动方式

原有 GUI 文件全部保留，新版 GUI 使用独立文件名，二者可同时安装：

| 用途 | launch | GUI 脚本 |
|---|---|---|
| 新版完整启动 | `elfin_gui_new.launch.py` | `elfin_gui_new.py` |
| 新版只启动界面 | `elfin_gui_only.launch.py` | `elfin_gui_new.py` |
| 原有真实机器人 GUI | `elfin_gui.launch.py` | `elfin_gui.py` |
| 原有 fake GUI | `fake_elfin_gui.launch.py` | `elfin_gui.py` |

完整启动新版控制器 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

后台驱动和 `elfin_sdk_node` 已运行时只启动新版界面：

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py
```

启动原有 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py
```

启动原有 fake GUI：

```bash
ros2 launch elfin_basic_api fake_elfin_gui.launch.py
```

旧 launch 只启动旧 GUI，仍需按原功能包流程启动其 Basic API、MoveIt、TF、轨迹控制器和 IO 服务。

## 8. 关键源码位置

| 文件 | 内容 |
|---|---|
| `elfin_basic_api/scripts/elfin_gui_new.py` | 新版 wxPython GUI、ROS bridge、按钮状态机和页面逻辑 |
| `elfin_basic_api/scripts/elfin_gui.py` | 原有 GUI，保留用于兼容和对照 |
| `elfin_basic_api/launch/elfin_gui_new.launch.py` | 完整控制器链路与新版 GUI 启动 |
| `elfin_basic_api/launch/elfin_gui_only.launch.py` | 只启动新版 GUI |
| `elfin_basic_api/launch/elfin_gui.launch.py` | 原有真实机器人 GUI 启动 |
| `elfin_controller_driver/src/sdk_node.cpp` | ROS service/topic、HRIF SDK 调用和运动 watchdog |
| `elfin_controller_driver/include/elfin_controller_driver/rt_info_client.hpp` | 10004 状态接收接口 |
| `elfin_robot_msgs/msg/` | GUI 使用的状态消息 |
| `elfin_robot_msgs/srv/` | GUI 使用的自定义服务 |
