# Elfin GUI 界面功能与 SDK 实现说明

## 一、实现原则

`elfin_gui.py` 是当前正式 GUI 源码和安装入口。它负责界面显示和用户操作，不直接加载 `libHR_Pro.so`，也不直接连接控制器端口。GUI 通过 ROS 2 service/topic 与 `elfin_controller_driver` 中的 `elfin_sdk_node` 通信，所有 HRIF SDK 调用由 C++ SDK 节点统一完成。

```text
elfin_gui.py（wxPython + rclpy）
        │ ROS 2 service / topic
        ▼
elfin_sdk_node（C++）
        ├─ HRIF SDK，10003：控制和设置命令
        ├─ 10004：机器人状态、IO、Brake 状态
        └─ controller_manager：ROS 控制权切换

ros2_control hardware
        ├─ 8893：高速关节/TCP 实际状态
        └─ 8892：ROS 控制模式下的 ServoJ/SpeedJ 命令
```

GUI 主线程运行 `wx.App.MainLoop()`；rclpy executor 放在后台线程。ROS 回调不得直接修改 wx 控件，必须使用 `wx.CallAfter()`。所有 service 使用 `call_async()`，禁止阻塞 GUI 主线程。

## 二、主页面

### 1. 界面上端

#### 1.1 Model

连接成功后显示机器人型号校验值，例如 `E05`。

实现方式：GUI 直接读取 launch 传入的 `robot_model` 参数并显示，不需要为这一项新增话题。原因是 controller 模式启动前，`elfin_sdk_preflight` 已经调用 `HRIF_ReadRobotModel`，并确认控制器型号与 `robot_model` 一致；校验失败时后续节点不会启动。因此 GUI 能运行时，launch 参数就是已经通过校验的型号。

如果未来要求 GUI 同时显示“启动参数型号”和“控制器原始型号”，才需要让常驻 SDK 节点发布 controller_info；当前需求不需要。

> [问题] `hardware_type:=controller`时，launch会先运行`elfin_sdk_preflight`，通过SDK 10003完成以下检查：

1. 调用`HRIF_ReadRobotModel`读取控制器机型；
2. 将控制器机型与启动参数`robot_model`比较，比较时忽略大小写、下划线和连字符；
3. 调用`HRIF_ReadVersion`读取控制器版本；
4. 要求控制器版本不低于`6.5.20d`。

只有两项检查全部成功后，才启动`ros2_control_node`、状态广播器、运动控制器和常驻SDK节点。成功日志示例：

```text
Preflight passed: model='E05', version='6.5.20d' (required >= 6.5.20d)
Elfin controller preflight passed; starting ros2_control
```

> [回答] 你说得对。Preflight 是一次性进程，结束后没有持续发布“校验结果”，GUI 不能直接订阅它；但校验成功后 `robot_model` 已经可信，所以 GUI 直接读取同一个 launch 参数即可。这里已取消“必须新增话题”的方案。只有以后需要动态读取控制器原始值时才新增话题。

#### 1.2 IP

显示机器人 IP，例如 `192.168.56.103`。

实现方式：从 launch 参数 `robot_ip` 读取，或由上述 controller_info 话题提供，不在 Python 文件中写死。

#### 1.3 SDK

连接成功显示 `SDK Connected`，否则显示 `SDK Disconnected`。

实现方式：订阅 `/elfin_sdk/robot_status` 的 `sdk_connected`。同时使用本地单调时钟检查消息新鲜度；超过 1 秒没有收到 10004 状态时显示 Disconnected/State Lost，并禁用危险操作。

#### 1.4 Servo

使能状态显示 `Servo On`，去使能状态显示 `Servo Off`。

实现方式：订阅 `/elfin_sdk/robot_status.enabled`。按钮调用成功后不能直接修改显示，必须等待控制器状态反馈。

#### 1.5 Fault

有错误显示 `Fault`，否则显示 `No Fault`。

实现方式：订阅 `/elfin_sdk/robot_status.error`，错误详情使用 `error_code` 和 `error_axis`。清错服务返回成功后仍要等待 error 变为 false。

#### 1.6 Stop

点击后停止机器人运动。

现有接口：

```text
/elfin_sdk/stop
std_srvs/srv/Trigger
HRIF_GrpStop(boxID, rbtID)
```

Stop 应尽可能始终可用。按下后还要取消 GUI 的点动定时器和 Hold-to-run 定时器。

### 2. 界面上半部分点动面板

#### 2.1 J1-J6

第一个方格显示实际关节角度，单位为 °。减号和加号分别进行负向和正向关节点动；中间方格输入目标关节角度。输入六个关节角后，长按 `Hold-to-run to target` 到达目标。

实际值实现：订阅 `/elfin_sdk/realtime_state.joint_position_actual`。该消息单位为 rad，GUI 显示前乘 `180 / π` 转成 degree。

点动 SDK：

```text
HRIF_LongJogJ(boxID, rbtID, axis, direction, state)
HRIF_LongMoveEvent(boxID, rbtID)
HRIF_GrpStop(boxID, rbtID)
```

当前实现已新增 `/elfin_sdk/jog`：

```text
# elfin_robot_msgs/srv/Jog.srv
uint8 mode       # 0: joint，1: cartesian
uint8 axis       # 0..5
int8 direction   # -1 或 +1
uint8 action     # 0: stop，1: start，2: keepalive
---
bool success
string message
```

鼠标按下发送 start，按住期间每 200 ms 发送 keepalive，鼠标抬起或离开按钮发送 stop。SDK 节点超过 500 ms 没收到 keepalive 必须主动停止，避免 GUI 崩溃后机器人继续运动。

目标运动已新增 `/elfin_sdk/move_target`，内部使用 `HRIF_WayPoint`。请求中的关节目标使用 degree。手动输入 J1-J6 或 XYZ/RX/RY/RZ 只改变输入框，不发送运动命令；只有按住 `Hold-to-run to target` 后才开始向已输入目标运动，松开按钮立即调用 `HRIF_GrpStop`。驱动侧 watchdog 在 500 ms 内收不到 keepalive 时主动停止。

接口为：

```text
# elfin_robot_msgs/srv/MoveTarget.srv
uint8 mode          # 0: joint，1: cartesian，2: home
float64[6] target   # joint: degree；cartesian: mm/mm/mm/degree/degree/degree
uint8 action        # 0: stop，1: start，2: keepalive
bool hold_required  # GUI 的目标、Home、Z 对齐均为 Hold-to-run=true
---
bool success
string message
```

Hold-to-run 按住期间每 200 ms 发送 keepalive；SDK 节点超过 500 ms 未收到时调用 Stop。Home 使用 mode=2、hold_required=true，目标取六个 0，松开 Home 按钮立即停止。
> [问题]现在没有关节运动接口吗？肯定有啊！

> [回答] 有关节运动接口，但要区分三层：① SDK 头文件已有 `HRIF_LongJogJ`、`HRIF_WayPoint` 等；② ros2_control 已有 FollowJointTrajectory/位置控制接口；③ 本次已在 `elfin_sdk_node` 中把 SDK 点动和 SDK 到点运动封装成 ROS 2 service。原 GUI 走 MoveIt/旧 teleop 服务，新 GUI 现已直接调用 SDK 的 ROS 2 封装。

#### 2.2 X/Y/Z

显示实际末端位置，单位为 mm。减号和加号进行笛卡尔负向/正向点动；中间方格输入目标 XYZ。

实际值实现：订阅 `/elfin_sdk/realtime_state.tcp_position_actual[0:3]`，ROS 消息为 m，显示前乘 1000 转成 mm。

点动使用 `HRIF_LongJogL`，axis 0/1/2 对应 X/Y/Z，通过同一个 `/elfin_sdk/jog` 服务实现。

#### 2.3 RX/RY/RZ

显示实际末端姿态，单位为 °。减号和加号进行笛卡尔旋转点动；中间方格输入目标 RX/RY/RZ。

实际值实现：订阅 `/elfin_sdk/realtime_state.tcp_position_actual[3:6]`，rad 转 degree。点动使用 `HRIF_LongJogL`，axis 3/4/5 对应 RX/RY/RZ。

笛卡尔目标运动复用 `/elfin_sdk/move_target`，请求单位为 mm、mm、mm、degree、degree、degree，SDK 节点内部调用 `HRIF_WayPoint`。

#### 2.4 Synchronize

点击后同步真实位姿。

该操作不发送运动命令。GUI 把最近一次 `/elfin_sdk/realtime_state` 的实际 J1-J6、XYZ、RX/RY/RZ 转换单位后写入目标输入框。

#### 2.5 Z-axis Alignment

点击后执行 Z 轴对齐。

Z 对齐复用 `/elfin_sdk/move_target` 的 `MODE_ALIGN_Z`。驱动先调用 `HRIF_MoveAlignToZ`，使用 TCP 名称 `TCP`、UCS 名称 `Base` 计算对齐后的关节目标，再通过 `HRIF_WayPoint` 执行。按住按钮期间发送 keepalive，松开后调用 `HRIF_GrpStop`；驱动同时检查机器人就绪、ROS 控制权和 Free Drive 状态。

#### 2.6 Home

点击后回到默认位姿。

Home 默认目标已确认是六个关节角全部为 0°。按住 Home 时复用 `/elfin_sdk/move_target`，目标为 `[0, 0, 0, 0, 0, 0]`；松开立即停止。该值仍建议作为 `home_joint_degrees` 参数保存，默认写六个 0，以便以后无需改代码即可调整。
> [问题]解释这里，没看懂

> [回答] SDK 头文件中没有明确的 `HRIF_GoHome()`，所以 Home 的实现就是把已确认的 `[0, 0, 0, 0, 0, 0]` 作为关节目标，通过 `/elfin_sdk/move_target` 调用 SDK 到点运动。参数仍放在配置中，目的是将来修改默认姿态时不必修改 Python/C++ 源码。

> [已确认] Home 使用 `[0, 0, 0, 0, 0, 0]`，不再等待各机型分别提供默认角度。

#### 2.7 Velocity Scaling

滑块选择速度，右侧方格显示实际速度百分比。

现有接口：

```text
/elfin_sdk/set_speed_ratio
elfin_robot_msgs/srv/SetFloat64
HRIF_SetOverride(boxID, rbtID, ratio)
```

GUI 的 1%～100% 除以 100 后发送为 0.01～1.0。右侧显示使用 `/elfin_sdk/realtime_state.speed_scaling * 100%`，不使用本地滑块值冒充控制器反馈。滑动时建议 100～200 ms 防抖，避免产生过多 service 请求。

### 3. 界面下部分功能面板

#### 3.1 Servo On

去使能状态下点击进入使能状态。

```text
/elfin_sdk/set_enabled
std_srvs/srv/SetBool {data: true}
HRIF_GrpEnable
```

请求期间按钮禁用；最终显示由 robot_status.enabled 决定。

#### 3.2 Servo Off

使能状态下点击进入去使能状态。

```text
/elfin_sdk/set_enabled
std_srvs/srv/SetBool {data: false}
HRIF_GrpDisable
```

现有 SDK 节点会先停止 ROS motion controller，再去使能。

#### 3.3 Clear Fault

有报错时点击清错。

```text
/elfin_sdk/reset
std_srvs/srv/Trigger
HRIF_GrpReset
```

失败时用对话框显示 response.message，不在左下角显示状态小字。

#### 3.4 Brake

点击跳转 Brake 页面。Servo On 时进入不可操作状态；去使能或符合安全条件时进入可操作状态。具体实现见本文 Brake 页面部分。

#### 3.5 Free Drive

点击进入或退出零力示教模式。

```text
/elfin_sdk/set_freedrive
std_srvs/srv/SetBool
HRIF_GrpOpenFreeDriver / HRIF_GrpCloseFreeDriver
```

按钮文字由 `/elfin_sdk/robot_status.freedrive` 决定。进入 Free Drive 前，现有 SDK 节点会停止 ROS motion controller。Free Drive 与 ROS Control、SDK Jog 必须互斥。

#### 3.6 ROS Active/Inactive

ROS 控制状态显示 `ROS Inactivate`，点击退出；非 ROS 控制状态显示 `ROS Activate`，点击进入。

```text
/elfin_sdk/set_ros_control
std_srvs/srv/SetBool
```

进入 ROS control 前必须满足：已使能、无故障、未暂停、Brake 已释放、未处于 Free Drive。现有 SDK 节点已执行这些检查。GUI 还需要控制模式反馈；当前消息没有 `ros_control_active` 字段，建议加入 robot_status 或新增控制模式话题。

#### 3.7 Safety Level

点击跳转 Safety Level 页面。

#### 3.8 Set I/O

点击跳转 Set I/O 页面。

#### 3.9 Payload

点击跳转 Payload 页面。

#### 3.10 TCP

点击跳转 TCP 页面。

## 三、Safety Level 页面

### 1. Back

返回主页面，不产生 SDK 命令。

### 2. 0～5 等级

点击某个等级后，从 0 到所选等级变黄。该界面实际对应碰撞灵敏度：0 最灵敏，5 最不灵敏。

SDK 接口：

```text
HRIF_SetCollideLevel(boxID, rbtID, level)
HRIF_GetCollideLevel(boxID, rbtID, level)
```

当前驱动没有对应 ROS 接口，需要新增 `/elfin_sdk/set_collision_level` 和 `/elfin_sdk/get_collision_level`。进入页面时必须读取并显示控制器实际等级，不设置 GUI 默认等级。
> [问题]没有对应接口要怎么办？需要知道什么内容和文档？默认值为3

> [回答] SDK 已经有接口，缺的是 ROS 2 封装。可在 `sdk_node.cpp` 新增服务：设置时调用 `HRIF_SetCollideLevel`，读取时调用 `HRIF_GetCollideLevel`。现有 `SetInt16.srv` 可用于设置；读取建议新增 `GetInt32.srv`。界面不使用默认值 3：读取成功前显示未知并禁用 Save，读取成功后显示控制器实际值。现有 `HR_Pro.h` 已给出范围 0～5 和灵敏度方向，基本足够实现。

### 3. Save / Cancel

Save 调用 set_collision_level，成功后再次读取确认；Cancel 恢复最近一次从控制器读取成功的值，而不是固定恢复 0。

已确认只有去使能状态才允许设置 Safety Level。Servo On 时等级可以读取和显示，但 Save 必须禁用；调用服务前 SDK 节点也要再次检查 robot enabled 状态，不能只依赖 GUI 禁用按钮。

## 四、Payload 页面

### 1. Name

固定显示 `Payload0`。注意 `HRIF_SetPayload` 没有名称参数，因此 Payload0 当前只是 GUI 显示名，不代表控制器中创建了同名配置。

### 2. Payload

输入负载质量，单位 kg。

### 3. CoG

输入 CX、CY、CZ，单位 mm。

SDK 接口：

```text
HRIF_SetPayload(boxID, rbtID, mass, cx, cy, cz, option)
HRIF_ReadPayload(boxID, rbtID, mass, cx, cy, cz)
HRIF_ReadMaxPayload(boxID, rbtID, max_payload)
```

当前驱动没有对应 ROS 服务，需要新增 `/elfin_sdk/set_payload` 和 `/elfin_sdk/get_payload`。保存前检查数值有限、mass ≥ 0 且不超过最大负载。
> [问题]没有对应接口要怎么办？需要知道什么内容和文档？

> [回答] 同样是“SDK 已有、ROS 2 未封装”。需要新增 `SetPayload.srv` 和 `GetPayload.srv`，在 `sdk_node.cpp` 分别调用 `HRIF_SetPayload`、`HRIF_ReadPayload`，并可调用 `HRIF_ReadMaxPayload` 做范围校验。现有 `HR_Pro.h` 已说明质量单位 kg、重心单位 mm 和 option 含义。实现前还要确认：界面 Save 是否需要写数据库（option 通常取 1）、控制器允许的最大负载，以及设置 Payload 是否要求特定 Servo 状态。

### 4. Back

返回主页面。存在未保存修改时应提示保存、放弃或取消返回。

### 5. Save / Cancel

Save 调用 set_payload，成功后重新读取确认；Cancel 恢复最近一次控制器确认值。

已确认只有去使能状态才允许设置 Payload。Servo On 时页面可读取和显示实际参数，但 Save 必须禁用，SDK 节点也必须进行同样的状态检查。

## 五、Set I/O 页面

### 1. DO0-7

有输出时显示灰色，否则显示白色。只订阅 `/elfin_sdk/io_state.digital_outputs`，不响应点击，不发送写 IO 服务。

### 2. CO0-7

只显示 `/elfin_sdk/io_state.configurable_outputs`，不提供修改功能。

### 3. EndDO0-2

只显示 `/elfin_sdk/end_io_state.digital_outputs`，不提供修改功能。

### 4. Back

返回主页面。

> [问题]这页不需要save/cancel，没有修改IO的功能，只显示接口的结果

> [回答] 已按你的确认修改：Set I/O 页面是只读状态页，不需要 Save/Cancel，DO、CO、EndDO 控件也不发送写命令。页面只订阅 10004 经 SDK 节点发布的两个 IO topic 并更新颜色。名称 `Set I/O` 容易被理解为可设置，后续可向带教确认是否改成 `I/O Status`，但不影响通信实现。

## 六、TCP 页面

页面标题显示 `TCP Offset from Flange`。这里配置的是当前工具坐标系相对于机器人
法兰盘中心的偏置，不是机器人当前末端在 Base 下的实时位姿。六个输入框分别为
X/Y/Z（mm）和 RX/RY/RZ（degree）。进入页面调用 `HRIF_ReadCurTCP` 只用于回填
控制器当前配置，Save 使用 `HRIF_SetTCP` 写入用户编辑后的偏置。

### 1. Name

固定显示 `TCP0`，只配置当前 TCP 六参数，不创建或持久化控制器命名 TCP，因此不使用 `HRIF_ConfigTCP` 或 `HRIF_SetDftTCP`。
> [问题]没明白什么意思？不是末端执行器吗

> [回答] TCP 是末端执行器上的“工具中心点坐标系”，不是末端执行器硬件本身。比如夹爪尖端相对机器人法兰的 X/Y/Z/RX/RY/RZ 偏移就是 TCP。已确认这里只配置当前 TCP，`TCP0` 只是界面名称，直接使用 `HRIF_SetTCP` 即可，不创建控制器命名配置。

### 2. X/Y/Z

输入 TCP 平移，单位 mm。

### 3. RX/RY/RZ

输入 TCP 姿态，单位 °。

现有保存接口：

```text
/elfin_sdk/set_tcp
elfin_robot_msgs/srv/SetPose
HRIF_SetTCP
```

SetPose 请求使用 SDK 原生单位：mm 和 degree。为了进入页面时显示真实值，还需新增 `/elfin_sdk/get_tcp`，内部调用 `HRIF_ReadCurTCP`。
> [问题]没有对应接口要怎么办？需要知道什么内容和文档？这里不是配置TCP吗

> [回答] 是配置当前 TCP。写接口已经存在：`/elfin_sdk/set_tcp` 调用 `HRIF_SetTCP`，Save 可以直接使用。缺少的是“读取当前 TCP”的 ROS 服务，用于打开页面时显示控制器现值以及 Cancel 恢复现值；新增 `/elfin_sdk/get_tcp` 并调用 `HRIF_ReadCurTCP` 即可。已确认不需要命名配置，因此无需 `HRIF_ConfigTCP` 文档。

### 4. Back

返回主页面。存在未保存修改时提示用户。

### 5. Save / Cancel

Save 调用 set_tcp，成功后重新读取当前 TCP；Cancel 恢复最近一次读取成功的值。

已确认只有去使能状态才允许设置 TCP。Servo On 时允许读取和查看，但 Save 禁用；SDK 节点处理 set_tcp 时也要检查 enabled=false。

## 七、Brake（Servo On）页面

### 1. 页面提示

显示：

```text
Currently not in de-energized state nor safety light-guard stopped jog-enabled
state. Operation is unavailable.
```

### 2. 六轴状态

六个轴显示灰色 `Brake Released`，不可点击。界面状态必须来自控制器反馈，不能只根据用户刚刚点击 Servo On 推断。

### 3. Back

返回主页面。

## 八、Brake 页面

### 1. 页面提示

显示允许修改状态和危险警告：

```text
Currently in De-energized or safety-light-guard stopped state, jog operation is
allowed. Click to change status.
⚠ Joints may drop under gravity after brake release. Please support the
manipulator manually!
```

当前 `ElfinRobotStatus.safeguard_stop` 在驱动中固定为 false。实现“安全光幕停止且允许点动”条件前，必须确认 10004 字段或可靠 SDK 读取方式，不能绕过此安全条件。

### 2. 六轴 Brake

SDK 接口：

```text
HRIF_OpenBrake(boxID, rbtID, axis)   # 松闸
HRIF_CloseBrake(boxID, rbtID, axis)  # 抱闸
HRIF_ReadBrakeStatus(...)            # 读取六轴状态
```

当前驱动没有单轴 Brake ROS 服务，需要新增：

```text
# elfin_robot_msgs/srv/SetBrake.srv
uint8 axis
bool release
---
bool success
string message
```

服务名 `/elfin_sdk/set_brake`。

10004 已解析 `BrakeState`，但当前 `/elfin_sdk/robot_status` 只有总体 `braking`。需要新增 `/elfin_sdk/brake_state`：

```text
std_msgs/Header header
bool[6] released
```

Brake 页面采用维护型按住操作，只有同时收到有效状态且满足以下全部条件时才允许
`OpenBrake`：SDK 已连接、Servo Off、机器人静止、无故障、已收到逐轴 Brake
反馈。按下某轴按钮调用 `OpenBrake`，松开或鼠标捕获丢失时调用 `CloseBrake`。
调用成功后不立即本地改色，必须等待 `/elfin_sdk/brake_state` 的真实反馈；状态未知
或断线时六轴全部禁用。若按住期间控制器状态发生变化，禁止新的 OpenBrake，但松手
仍尝试 CloseBrake。

当前尚未确认控制器是否要求持续发送命令才能保持松闸，因此 GUI 暂按“按住松闸、
松手请求抱闸”的保守交互实现。`CloseBrake` 返回 40017 或其他错误时，GUI 原样显示
SDK 返回的错误信息；不得自动调用 `HRIF_Blackout`、安全光幕或其他 FSM 切换接口。

虚拟控制器 HR6.5.22a 已实测：调用 `HRIF_OpenBrake(..., axis=0)` 后，10004 与
`HRIF_ReadBrakeStatus` 的 Axis1 原始值均由 0 变为 1。因此逐轴 `BrakeState`
应解释为手动松闸标志：0=未手动松闸，非 0=已手动松闸；它不是控制器 FSM 的
`Braking` 状态。机器人正常使能时，即使原始值为 0，物理制动器也由控制器释放。

从当前驱动逻辑看，`sdk_node.cpp` 把 `BrakeState` 中任一非零值解释为 `braking=true`，因此现有代码隐含假设是 0=Released、非零=Engaged；这只是代码现状，不是实测结论，仍需按下面流程验证。
> [问题]没有对应接口要怎么办？需要知道什么内容和文档？

> [回答] SDK 已有 `HRIF_OpenBrake`、`HRIF_CloseBrake`、`HRIF_ReadBrakeStatus`，ROS 2 service 和逐轴状态 topic 已新增。实测已确认 `OpenBrake` 使对应轴原始值从 0 变为 1，且 10004 与 `HRIF_ReadBrakeStatus` 一致。尚未确认的是 `CloseBrake` 的控制器 FSM 前置条件；在 `RobotDisable`、已上电且 `HRIF_ReadRobotState` 返回 `braking=1` 时，控制器仍返回 40017（不在 Braking state）。因此不能把 Servo Off 当成充分条件，也不能在没有 SDK/带教确认时自动调用 `HRIF_Blackout`。

> [测试状态] 虚拟控制器当前可达。Servo Off 后 `OpenBrake(axis=0)` 成功，Axis1 原始值 0→1；随后 `CloseBrake(axis=0)` 返回 40017。只读诊断结果为 `FSM=24 RobotDisable`、`electrified=1`、`braking=1`、逐轴值 `[1,0,0,0,0,0]`。下一步需带教或对应版本 SDK 文档说明：如何进入 `CloseBrake` 所要求的内部 Braking FSM，以及 `Blackout`、安全光幕状态与 Open/CloseBrake 的准确关系。


### 3. Back

返回主页面。

## 九、通信安全与状态管理

### 1. 控制模式互斥

ROS Control、Free Drive、SDK Jog/目标运动三者互斥。开始 SDK 点动、Home、Z 对齐或 Hold-to-run 前，必须先调用 `/elfin_sdk/set_ros_control false` 并等待成功。

### 2. 状态反馈原则

Servo、Fault、Free Drive、IO、Brake、速度等显示都以控制器 topic 反馈为准。service 返回 success 只表示命令被接受，不表示物理状态已完成。

### 3. 超时和断线

- 普通 service 建议 2 秒应用层超时，控制器切换建议 5 秒。
- 不在 wx 主线程调用 `wait_for_service()`。
- 10004 或 8893 状态超过 1 秒未更新时，禁用所有运动和设置功能。
- SDK 重连后重新读取 model、TCP、Payload、collision level 和 Brake，不使用旧缓存。
- 窗口关闭、断线、故障、去使能和 watchdog 超时都必须停止点动及目标运动。

### 4. 错误显示

通信失败或 SDK 返回非零错误码时，用 wx 对话框显示 service response.message。界面不设置底部状态栏，不显示 `UI only`、`Showing ... page` 等小字。

## 十、建议实现顺序

1. 先定义并编译缺少的 ROS msg/srv，固定 GUI 与驱动之间的接口。
2. 在现有 `sdk_node.cpp` 实现并用命令行独立验证非运动 SDK 服务。
3. 给 GUI 增加 ROS 后台 executor，先接状态显示，再接已有非运动服务。
4. 接入 Safety、Payload、当前 TCP 和 Brake 等新增服务。
5. 最后实现 Jog、MoveTarget、Home、Z-axis Alignment、Hold-to-run 和 watchdog。
6. 完成断线、退出、超时和控制权冲突测试后再接入 launch。
7. 虚拟控制器完整验收后才测试真实机器人。详细文件级步骤见第十二节。

## 十一、实现前需要确认的问题

1. 10004 `BrakeState` 中 0/1 分别代表抱闸还是松闸。
2. 安全光幕停止且允许点动的可靠状态字段来源。
3. `HRIF_MoveAlignToZ` 使用 TCP=`TCP`、UCS=`Base` 已确认；仍需通过虚拟控制器确认函数的完成/停止语义。
4. Hold-to-run 的交互已确认；仍需确认控制器侧使用 `HRIF_WayPoint` 加释放 Stop 是否与控制器界面内部实现一致。
5. Payload0 是否只表示当前值，还是要求持久化命名配置。
6. Safety Level 和 Payload 是否写入控制器持久配置；Safety Level、Payload、TCP 都已确认只有 Servo Off 才允许设置。

## 十二、后续实现需要的内容和文件架构

### 1. 已有内容，可以直接使用

- `elfin_controller_driver/src/sdk_node.cpp`：已有 Servo、Stop、Reset、Free Drive、ROS Control、速度比例、TCP 设置和 IO 状态发布。
- `elfin_controller_driver/src/controller_hardware.cpp`：已有 8893 实时状态和 8892 ROS 控制指令。
- `elfin_controller_driver/src/rt_info_client.cpp`：已有 10004 JSON、IO 和 BrakeState 解析。
- `elfin_robot_msgs/msg/ElfinRealtimeState.msg`：提供关节和 TCP 实际状态。
- `elfin_robot_msgs/msg/ElfinRobotStatus.msg`：提供 Servo、Fault、Free Drive 等状态。
- `elfin_robot_msgs/msg/ElfinIOState.msg`、`ElfinEndIOState.msg`：提供只读 IO 状态。
- `elfin_basic_api/scripts/elfin_gui.py`：当前正式 GUI 源码和安装入口。

### 2. 需要带教或 SDK 负责人提供/确认的内容

- BrakeState 0/1 定义和 Brake 的完整安全操作条件。
- Z-axis Alignment 的完成、取消和异常停止语义；TCP=`TCP`、UCS=`Base` 已确认。
- `HRIF_WayPoint + HRIF_GrpStop` 是否就是控制器 Hold-to-run 的内部实现。
- Payload 的 option、持久化要求和各机型最大负载规则。
- Payload0 是否需要作为控制器命名配置保存；TCP 已确认只配置当前值。
- Safety Level 和 Payload 是否持久化；Servo Off 限制已确认。

### 3. 建议新增的消息和服务文件

```text
elfin_robot_msgs/
├── msg/
│   └── ElfinBrakeState.msg       # 六轴 Brake 反馈
└── srv/
    ├── Jog.srv                   # 关节/笛卡尔 start、stop、keepalive
    ├── MoveTarget.srv            # 关节/笛卡尔目标运动
    ├── GetInt32.srv              # 读取碰撞等级
    ├── SetPayload.srv            # 设置 Payload
    ├── GetPayload.srv            # 读取 Payload
    ├── GetPose.srv               # 读取当前 TCP
    └── SetBrake.srv              # 单轴松闸/抱闸
```

新增文件后修改 `elfin_robot_msgs/CMakeLists.txt` 注册接口。

### 4. 推荐最终代码结构

```text
elfin_basic_api/
├── README.md
├── README_cn.md
├── elfin_gui.md
├── launch/
│   ├── elfin_gui.launch.py       # 控制器 bringup + GUI
│   └── elfin_gui_only.launch.py  # 只启动 GUI
└── scripts/
    └── elfin_gui.py               # controller GUI 源码和安装入口

elfin_controller_driver/
└── src/
    └── sdk_node.cpp               # 在现有常驻节点内继续增加 SDK 服务和 watchdog
```

GUI ROS bridge 和 wx 页面当前位于同一源码中，由 CMake 以软链接方式安装。C++ 侧暂不拆分 `sdk_node.cpp`，避免为了 GUI 重构当前节点类和启动方式；SDK 调用仍只留在 C++ 驱动侧。

旧 MoveIt GUI 已删除。`elfin_gui.launch.py` 统一传递 `robot_model`、`robot_ip` 和校验参数并启动 controller bringup；已有 bringup 时使用 `elfin_gui_only.launch.py`，避免重复连接控制器。

### 5. 具体编码顺序

1. 先在 `elfin_robot_msgs` 增加后续确定需要的 msg/srv 并单独编译消息包；这样驱动和 GUI 都能基于稳定接口开发。
2. 在现有 `sdk_node.cpp` 增加 Get TCP、collision level、Payload、Brake 等非运动服务，先用 `ros2 service call` 和 topic 在虚拟控制器独立验证，不立即接 GUI。
3. GUI ROS bridge 当前直接实现在 `scripts/elfin_gui.py`，rclpy executor 运行于后台线程，所有 ROS 回调通过 `wx.CallAfter()` 更新界面。
4. 已接入真实状态显示：`/elfin_sdk/robot_status` 显示 SDK、Servo、Fault、Free Drive 和 ROS Control；`/elfin_sdk/realtime_state` 显示六轴实际角度、实际 TCP 与速度比例；`/elfin_sdk/brake_state` 显示逐轴 Brake。GUI 将关节 rad 转为 deg、TCP 平移 m 转为 mm、TCP 转角 rad 转为 deg。实时位姿由 ros2_control 硬件节点发布，只有完整 bringup 运行时存在。
5. 已接已有服务：Servo、Clear Fault、Stop、Free Drive、ROS Control、速度比例、Get TCP 和 Set TCP。所有调用均为异步，service 成功不直接修改状态显示，而是等待后续 topic；TCP 页面进入时读取当前 TCP，只有 SDK 在线、Servo Off 且机器人静止时允许 Save。Safety 和 Payload 仍按后续步骤接入。
6. 在 `sdk_node.cpp` 新增 Jog 服务、200 ms keepalive/500 ms watchdog，先用测试客户端验证按下、按住、释放、客户端崩溃和断网，再连接 GUI 加减按钮。
7. 新增 MoveTarget、Home `[0,0,0,0,0,0]`、Z-axis Alignment（TCP=`TCP`、UCS=`Base`）和 Hold-to-run；输入值本身绝不触发运动，只有按钮按住期间运动。
8. 完成错误弹窗、输入校验、状态超时、断线停止、模式互斥和重连恢复。
9. 修改 `elfin_basic_api/CMakeLists.txt` 和 launch 安装正式入口，并删除旧 GUI/MoveIt GUI 路径。
10. 整包编译并在虚拟控制器完成正常、拒绝、断线、窗口关闭和模式冲突验收，最后才进行真实机器人低速测试。
