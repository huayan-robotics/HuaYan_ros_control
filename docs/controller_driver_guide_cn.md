# Elfin ROS 2 控制器驱动使用手册

本文档说明 Elfin 机械臂在 ROS 2 Humble 下的启动方式、控制类型、MoveIt/Gazebo
组合方式，以及驱动提供的服务和话题。

## 1. 系统结构

真实机械臂使用三个控制器端口：

| 端口 | 用途 |
|---|---|
| 8893 | 实时读取关节、TCP、力和控制器状态 |
| 8892 | `servoj`/`speedj` 实时运动命令 |
| 10003 | 厂商 SDK，使能、IO、TCP、示教等普通控制 |
| 10004 | 周期推送JSON机器人状态、电箱IO和末端IO |

主要 ROS 2 组件：

| 组件 | 功能 |
|---|---|
| `ElfinControllerHardware` | ros2_control 硬件插件，读取 8893、写入 8892 |
| `elfin_sdk_node` | 10003提供主动控制服务，10004接收并发布机器人/IO状态 |
| `controller_manager` | 管理轨迹、速度和状态控制器 |
| `joint_state_broadcaster` | 发布标准 `/joint_states` |
| `robot_state_publisher` | 根据 `/joint_states` 发布 TF |
| `move_group` | MoveIt 规划与轨迹执行 |
| RViz | 显示当前机器人状态和规划结果 |

## 2. 编译和环境

```bash
cd ~/elfin_ros2_ws
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

确认加载的是当前工作区：

```bash
ros2 pkg prefix elfin_robot_bringup
```

应输出：

```text
/home/cjw/elfin_ros2_ws/install/elfin_robot_bringup
```

支持的 `robot_model`：

```text
E03
E05
E05-L
E10
E10-L
E15
```

## 3. 统一启动入口

统一入口为：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py
```

启动参数默认从以下统一配置文件读取：

```text
elfin_robot_bringup/config/elfin_control.yaml
```

修改该文件后重新构建`elfin_robot_bringup`，后续启动命令不再需要携带机器人型号、控制类型和IP等参数。

主要参数：

| 参数 | 默认值 | 说明 |
|---|---:|---|
| `config_file` | 包内`config/elfin_control.yaml` | 统一启动配置文件路径 |
| `robot_model` | `E05` | 机器人型号（控制器 `typealias`） |
| `hardware_type` | `gazebo` | `controller`、`gazebo`、`ethercat` 或 `fake` |
| `robot_ip` | `10.20.200.3` | 真实控制器 IP |
| `control_mode` | `position` | `position`、`velocity` 或 `controller` |
| `update_rate` | `1000` | ros2_control读/写频率；1 ms控制器填`1000`，4 ms控制器填`250` |
| `servo_gain` | `8000` | 8892 `StartServo` 增益 |
| `lookahead_time` | `0.004` | ServoJ 前瞻时间，单位秒 |
| `servo_restart_idle_ms` | `100` | 位置命令停止变化达到该时间后，下一段轨迹重新调用 `StartServo` |
| `position_command_epsilon` | `1e-8` | 位置命令变化阈值，rad |
| `velocity_command_epsilon` | `1e-8` | 速度命令变化阈值，rad/s |
| `command_log_throttle_ms` | `100` | 8892日志采样周期；`0`输出每条指令，负值关闭 |
| `state_stale_timeout_ms` | `100` | 8893状态陈旧阈值，ms |
| `state_disconnect_timeout_ms` | `1000` | 8893断线重连阈值，ms |
| `enable_controller_validation` | `true` | 是否在启动ros2_control前校验控制器机型和版本 |
| `pushed_state_port` | `10004` | JSON状态推送TCP端口 |
| `pushed_state_socket_timeout_ms` | `100` | 10004单次接收超时，ms |
| `pushed_state_disconnect_timeout_ms` | `1000` | 10004持续无帧后的重连阈值，ms |
| `status_publish_rate` | `10.0` | `/elfin_sdk/robot_status`发布频率，Hz |
| `io_publish_rate` | `5.0` | 箱体与末端IO话题发布频率，Hz |

默认配置示例：

```yaml
elfin_control:
  robot_model: E05
  hardware_type: controller
  robot_ip: 10.20.215.133
  control_mode: position
  update_rate: 1000
  enable_controller_validation: true
  servo_gain: 8000
  lookahead_time: 0.004
  servo_restart_idle_ms: 100
  position_command_epsilon: 1.0e-8
  velocity_command_epsilon: 1.0e-8
  command_log_throttle_ms: 100
  state_stale_timeout_ms: 100
  state_disconnect_timeout_ms: 1000
  pushed_state_port: 10004
  pushed_state_socket_timeout_ms: 100
  pushed_state_disconnect_timeout_ms: 1000
  status_publish_rate: 10.0
  io_publish_rate: 5.0
```

配置文件提供日常启动值；显式传入的同名launch参数可以作为单次覆盖。未知字段、空值或非法枚举会使launch直接报错，避免拼写错误被静默忽略。

### 3.1 控制频率配置与校验

`update_rate`必须与控制器版本的控制周期一致。普通版本的8893数据包中
`cycle_time`为1 ms，使用1000 Hz；升级包名称带`4ms`的版本使用250 Hz：

| 控制器周期/版本 | `update_rate` |
|---|---:|
| 1 ms（版本名称不带`4ms`） | `1000` Hz |
| 4 ms（版本名称带`4ms`） | `250` Hz |

日常使用只修改统一配置文件中的这一处：

```yaml
# elfin_robot_bringup/config/elfin_control.yaml
elfin_control:
  update_rate: 1000   # 1 ms版本
```

4 ms版本改为：

```yaml
elfin_control:
  update_rate: 250
```

同一参数会同时传给`controller_manager.update_rate`和硬件插件的周期校验，
不需要修改`elfin_controller_position.yaml`、`elfin_controller_velocity.yaml`或
`elfin_controller_state_only.yaml`。如只想对一次启动临时覆盖，可使用：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  update_rate:=250
```

硬件插件连接8893后读取首个有效包中的`cycle_time`并进行一致性检查：1 ms只接受
1000 Hz，4 ms只接受250 Hz。配置不匹配、周期值未知或参数不是250/1000时，
驱动会输出`Control frequency mismatch`或`Unsupported 8893 cycle_time`并拒绝激活，
不会以错误频率继续运行。该检查直接使用控制器实时数据，不依赖
`enable_controller_validation`，因此关闭机型/版本校验也不会跳过频率校验。

如需保留多套配置，可以复制YAML并只指定配置文件：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  config_file:=/absolute/path/to/another_elfin_control.yaml
```

`fake` 目前仅保留为规划中的类型，统一入口尚未实现完整 fake bringup。

### 3.1 真实控制器启动前校验

`hardware_type:=controller`时，launch会先运行`elfin_sdk_preflight`，通过SDK 10003完成以下检查：

1. 调用`HRIF_ReadRobotModel`读取控制器机型；
2. 将控制器机型与启动参数`robot_model`比较，比较时忽略大小写、下划线和连字符；
3. 调用`HRIF_ReadVersion`读取控制器版本；
4. 要求控制器版本不低于`6.5.20d`。

只有两项检查全部成功后，才启动`ros2_control_node`、状态广播器、运动控制器和常驻SDK节点。成功日志示例：

```text
Preflight passed: model='E05', version='6.5.20d' (required >= 6.5.20d)
Elfin controller preflight passed; starting ros2_control
```

机型不匹配、版本过低、版本字符串无法解析或SDK读取失败时，launch会报错并终止，ROS不会获得机械臂控制权。例如：

```text
Preflight failed: controller model 'E10' does not match launch robot_model 'E05'
Preflight failed: controller version '6.5.20c' is older than required version 6.5.20d
```

校验开关默认为开启。需要跳过机型和版本校验时显式设置：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=position \
  robot_ip:=10.20.215.133 enable_controller_validation:=false
```

关闭后不会运行`elfin_sdk_preflight`，而是直接启动ros2_control，并输出：

```text
Elfin controller model/version validation is disabled
```

除非正在兼容无法正确返回机型或版本信息的旧控制器，否则建议保持开启。

### 3.2 新旧 GUI 启动方式

`elfin_basic_api` 同时保留原有 GUI 和新版控制器 GUI。编译后先加载当前工作区：

```bash
cd ~/elfin_ros2_ws
source /opt/ros/humble/setup.bash
source install/setup.bash
```

#### 新版 GUI：完整启动

以下命令包含 `elfin_robot_bringup/launch/elfin_control.launch.py`，会启动控制器
bringup、`elfin_sdk_node`、ros2_control 和新版 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 \
  robot_ip:=192.168.56.103
```

新版 GUI 源码为 `elfin_basic_api/scripts/elfin_gui_new.py`。它通过
`/elfin_sdk/*` topic/service 与 `elfin_sdk_node` 通信，不直接连接控制器端口。
完整启动默认使用 `hardware_type:=controller`、`control_mode:=controller`，并开启
机型和版本校验。

该GUI完整入口与统一bringup使用同一个控制频率配置。默认读取
`elfin_robot_bringup/config/elfin_control.yaml`中的`update_rate`，也可临时覆盖：

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103 \
  update_rate:=250
```

1 ms控制器使用1000 Hz，4 ms控制器使用250 Hz；不匹配时硬件拒绝激活并结束
完整启动。GUI-only入口不启动controller_manager，因此不接受`update_rate`。

#### 新版 GUI：只启动界面

如果控制器 bringup 和 `elfin_sdk_node` 已经在其他终端运行，只启动新版 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py
```

也可以直接运行：

```bash
ros2 run elfin_basic_api elfin_gui_new.py
```

只启动界面不会创建 `/elfin_sdk/*` 服务和状态话题；后台节点未运行时，GUI 会显示
服务不可用或状态失联。

#### 原有 GUI：真实机器人界面

原有启动文件和源码保持兼容：

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py
```

该 launch 启动原有 `elfin_basic_api/scripts/elfin_gui.py`，并设置
`use_fake_robot:=false`、`use_sim_time:=false`。也可直接运行原脚本：

```bash
ros2 run elfin_basic_api elfin_gui.py
```

原有 GUI 使用旧 Basic API、MoveIt、TF、IO 和轨迹控制接口，包括
`/joint_teleop`、`/cart_teleop`、`/stop_teleop`、`/home_teleop`、
`/read_di`、`/read_do`、`/write_do`、`/joint_states` 和
`elfin_arm_controller/follow_joint_trajectory`。该 launch 只启动旧 GUI，本身不会
启动上述后台节点；需要先按原功能包流程启动对应驱动和服务。

#### 原有 GUI：fake 界面

原 fake 启动文件仍然保留：

```bash
ros2 launch elfin_basic_api fake_elfin_gui.launch.py
```

它启动原有 `elfin_gui.py`，并设置 `use_fake_robot:=true`、
`use_sim_time:=true`。它不会自动启动 Gazebo、MoveIt、旧 Basic API 或 IO 服务，
这些依赖仍需按原仿真流程提前启动。

`elfin_basic_api.launch.py`是另一个历史入口，只启动旧
`elfin_basic_api_node`，并且当前模型资源硬编码为Elfin10。它不启动驱动、MoveIt、
GUI或仿真，不应作为新版GUI或统一机器人启动入口。

新旧文件对应关系如下：

| 用途 | launch | GUI 脚本 |
|---|---|---|
| 新版完整启动 | `elfin_gui_new.launch.py` | `elfin_gui_new.py` |
| 新版只启动界面 | `elfin_gui_only.launch.py` | `elfin_gui_new.py` |
| 原有真实机器人 GUI | `elfin_gui.launch.py` | `elfin_gui.py` |
| 原有 fake GUI | `fake_elfin_gui.launch.py` | `elfin_gui.py` |

## 4. 真实机械臂 + MoveIt + RViz

### 4.1 ROS 位置控制

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 \
  hardware_type:=controller \
  control_mode:=position \
  robot_ip:=10.20.215.133
```

此模式启动真实通信、ros2_control、MoveIt 和 RViz，不启动 Gazebo。

MoveIt 将轨迹发送到：

```text
/elfin_arm_controller/follow_joint_trajectory
```

位置命令通过8892转换成`StartServo`和`PushServoJ`。驱动不会在ros2_control的每个更新周期重复发送不变的位置：

1. 控制器激活时，以8893实际位置建立命令基准，不启动ServoJ；
2. MoveIt执行轨迹、目标位置首次发生变化时，先发送`StartServo`，再发送`PushServoJ`；
3. 同一段连续轨迹内，仅在位置数值变化时发送`PushServoJ`；
4. 位置停止变化达到`servo_restart_idle_ms`后，认为本段Servo流结束；
5. 下一次MoveIt轨迹产生新位置时，再次先发送`StartServo`，然后发送`PushServoJ`。

这样空闲时机器人不会因为重复收到相同ServoJ命令而一直显示为运动状态，多次规划执行也会为每段新轨迹重新启动Servo任务。

### 4.2 只读/控制器控制

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 \
  hardware_type:=controller \
  control_mode:=controller \
  robot_ip:=10.20.215.133
```

此模式下：

- 8893持续更新机械臂状态；
- `joint_state_broadcaster` 保持运行；
- RViz 和 MoveIt 中的当前状态跟随真实机械臂；
- `elfin_arm_controller` 被加载为 inactive；
- ROS 不向 8892发送运动命令；
- 可由示教器、控制器程序或零力示教移动机械臂。

需要切换到 ROS 控制时调用：

```bash
ros2 service call /elfin_sdk/set_ros_control \
  std_srvs/srv/SetBool "{data: true}"
```

真实控制器的轨迹控制器启动时先加载为 `inactive`。在`position`或`velocity`
模式下，如果收到的第一帧10004状态已经满足下列就绪条件，驱动会在控制器完成加载后
自动将其激活。这样既不会在状态未知时提前下发指令，也不需要对启动前已经使能就绪的
机械臂额外调用服务：

- 已收到有效的10004状态帧；
- 机械臂已经使能；
- 机械臂没有活动故障；
- 机械臂没有暂停；
- 六个关节制动器均已释放；
- 零力示教和力控自由驱动均未开启。

如果启动时机械臂尚未就绪，则本次启动不会在机械臂随后使能时自动取得控制权。
调用`set_enabled(true)`只负责使能机械臂，不会自动开启ROS控制。使能完成并在
`/elfin_sdk/robot_status`中确认`enabled: true`、`error: false`、`braking: false`
后，仍需显式调用`set_ros_control(true)`。机械臂去使能、进入故障/暂停/制动状态，
或者10004状态流断开时，驱动会自动停用ROS运动控制器；恢复就绪后也不会自动
重新激活，必须再次调用`set_ros_control(true)`。

释放 ROS 控制权：

```bash
ros2 service call /elfin_sdk/set_ros_control \
  std_srvs/srv/SetBool "{data: false}"
```

释放ROS控制权时驱动会对8892套接字执行`shutdown(SHUT_RDWR)`并关闭连接。
Ctrl+C、SIGINT、SIGTERM、硬件生命周期`deactivate/cleanup/shutdown/error`以及硬件插件
析构也会关闭8892、8893和10004连接。`kill -9`（SIGKILL）无法执行用户态清理代码，
只能依靠操作系统关闭文件描述符以及控制器端TCP超时/keepalive回收，因此不应作为正常
停止方式使用。

### 4.3 ROS 速度控制

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 \
  hardware_type:=controller \
  control_mode:=velocity \
  robot_ip:=10.20.215.133
```

速度控制器订阅：

```text
/elfin_velocity_controller/commands
```

发送示例，单位为 rad/s：

```bash
ros2 topic pub --once /elfin_velocity_controller/commands \
  std_msgs/msg/Float64MultiArray \
  "{data: [0.1, 0.0, 0.0, 0.0, 0.0, 0.0]}"
```

该模式面向直接速度控制。当前 MoveIt 执行配置使用
`elfin_arm_controller/FollowJointTrajectory`，因此 MoveIt 轨迹执行应使用 position 模式。

速度写入同样使用数值变化检测：初始零速度和未变化的速度不会重复发送；速度值变化时发送一次
`SpeedJ`，非零速度变为零时会发送一次零速度命令。相同数值即使被上层重复发布，也不会重复写入8892；持续时间由`speed_runtime`和控制器侧SpeedJ语义决定。

## 5. Gazebo + MoveIt + RViz 联合仿真

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 \
  hardware_type:=gazebo
```

该模式启动：

- Gazebo；
- `gazebo_ros2_control`；
- `joint_state_broadcaster`；
- `elfin_arm_controller`；
- MoveIt `move_group`；
- RViz。

该模式不会连接8892、8893、SDK 10003或状态端口10004。

## 6. 旧 EtherCAT 模式

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 \
  hardware_type:=ethercat
```

此入口保留旧 SOEM/EtherCAT 驱动兼容性。新项目建议使用 `controller` 模式。

## 7. 控制类型与控制器配置

| `control_mode` | ROS 控制器 | 8892 | MoveIt执行 | 用途 |
|---|---|---|---|---|
| `position` | `elfin_arm_controller` active | ServoJ | 支持 | MoveIt、轨迹控制 |
| `velocity` | `elfin_velocity_controller` active | SpeedJ | 不支持当前配置 | 实时关节速度控制 |
| `controller` | 轨迹控制器 inactive | 不发送 | 仅监视/规划 | 示教器、SDK、零力示教 |

配置文件：

| 文件 | 用途 |
|---|---|
| `elfin_controller_position.yaml` | 位置轨迹控制 |
| `elfin_controller_velocity.yaml` | 关节速度控制 |
| `elfin_controller_state_only.yaml` | 控制器控制/只读状态 |

检查控制器：

```bash
ros2 control list_hardware_components -v
ros2 control list_controllers
ros2 control list_hardware_interfaces
```

## 8. SDK 服务

以下服务仅在 `hardware_type:=controller` 时可用。

### 8.1 使能和运动状态

| 服务 | 类型 | 说明 |
|---|---|---|
| `/elfin_sdk/set_enabled` | `std_srvs/srv/SetBool` | `true` 使能，`false` 去使能 |
| `/elfin_sdk/stop` | `std_srvs/srv/Trigger` | 停止运动 |
| `/elfin_sdk/reset` | `std_srvs/srv/Trigger` | 复位机械臂 |
| `/elfin_sdk/pause` | `std_srvs/srv/Trigger` | 暂停控制器运动 |
| `/elfin_sdk/continue` | `std_srvs/srv/Trigger` | 继续已暂停的运动 |
| `/elfin_sdk/set_speed_ratio` | `elfin_robot_msgs/srv/SetFloat64` | 设置速度比例，范围0.01～1.0 |
| `/elfin_sdk/set_override` | `elfin_robot_msgs/srv/SetFloat64` | 速度比例兼容接口，功能同`set_speed_ratio` |

使能示例：

```bash
ros2 service call /elfin_sdk/set_enabled \
  std_srvs/srv/SetBool "{data: true}"
```

去使能：

```bash
ros2 service call /elfin_sdk/set_enabled \
  std_srvs/srv/SetBool "{data: false}"
```

设置 50% 速度比例：

```bash
ros2 service call /elfin_sdk/set_speed_ratio \
  elfin_robot_msgs/srv/SetFloat64 "{data: 0.5}"
```

`data: 0.5`表示50%。服务会检查输入范围；小于0.01或大于1.0时不会调用SDK，并返回失败。

### 8.2 控制权和示教模式

| 服务 | 类型 | 说明 |
|---|---|---|
| `/elfin_sdk/set_ros_control` | `std_srvs/srv/SetBool` | 激活或停止 ROS 运动控制器 |
| `/elfin_sdk/set_freedrive` | `std_srvs/srv/SetBool` | 普通零力示教 |
| `/elfin_sdk/set_force_freedrive` | `std_srvs/srv/SetBool` | 力控自由驱动 |

`freedrive`和`force_freedrive`是两个不同且互斥的模式。进入任一模式前驱动会先停止ROS运动控制器，8893、`/joint_states`、TF和RViz仍持续更新真实位置。

关闭示教模式时只关闭对应的SDK示教功能，**不会自动重新激活ROS运动控制器**，也不会恢复进入示教前的旧轨迹。需要再次由MoveIt控制时，必须显式调用`set_ros_control(true)`。重新激活时驱动先将命令接口同步到8893反馈的实际关节位置，避免机器人跳回示教前的位置。

零力示教：

```bash
ros2 service call /elfin_sdk/set_freedrive \
  std_srvs/srv/SetBool "{data: true}"

ros2 service call /elfin_sdk/set_freedrive \
  std_srvs/srv/SetBool "{data: false}"

ros2 service call /elfin_sdk/set_ros_control \
  std_srvs/srv/SetBool "{data: true}"
```

力控自由驱动：

```bash
ros2 service call /elfin_sdk/set_force_freedrive \
  std_srvs/srv/SetBool "{data: true}"
```

### 8.3 TCP 和 UCS

| 服务 | 类型 | 说明 |
|---|---|---|
| `/elfin_sdk/set_tcp` | `elfin_robot_msgs/srv/SetPose` | 按数值设置 TCP |
| `/elfin_sdk/set_ucs` | `elfin_robot_msgs/srv/SetPose` | 按数值设置 UCS |
| `/elfin_sdk/set_tcp_by_name` | `elfin_robot_msgs/srv/SetString` | 按控制器中的名称选择 TCP |
| `/elfin_sdk/set_ucs_by_name` | `elfin_robot_msgs/srv/SetString` | 按名称选择 UCS |

`SetPose.pose` 顺序为：

```text
[X, Y, Z, Rx, Ry, Rz]
```

服务沿用 SDK 原生单位：X/Y/Z 为 mm，Rx/Ry/Rz 为度。

```bash
ros2 service call /elfin_sdk/set_tcp \
  elfin_robot_msgs/srv/SetPose \
  "{pose: [0.0, 0.0, 150.0, 0.0, 0.0, 0.0]}"

ros2 service call /elfin_sdk/set_tcp_by_name \
  elfin_robot_msgs/srv/SetString "{data: TCP_1}"
```

### 8.4 IO

| 服务 | 类型 | 说明 |
|---|---|---|
| `/elfin_sdk/set_digital_io` | `elfin_robot_msgs/srv/SetDigitalIO` | 设置数字输出 |
| `/elfin_sdk/set_analog_io` | `elfin_robot_msgs/srv/SetAnalogIO` | 设置箱体模拟输出 |

数字 IO 的 `domain`：

| domain | 说明 |
|---|---|
| `box_do` | 控制箱普通数字输出 |
| `box_co` | 控制箱配置输出 |
| `end_do` | 机械臂末端数字输出 |

```bash
ros2 service call /elfin_sdk/set_digital_io \
  elfin_robot_msgs/srv/SetDigitalIO \
  "{domain: box_do, index: 0, value: true}"

ros2 service call /elfin_sdk/set_digital_io \
  elfin_robot_msgs/srv/SetDigitalIO \
  "{domain: end_do, index: 0, value: true}"

ros2 service call /elfin_sdk/set_analog_io \
  elfin_robot_msgs/srv/SetAnalogIO \
  "{index: 0, mode: 0, value: 5.0}"
```

模拟输出的 `mode` 和 `value` 含义以对应控制器型号的 SDK/电气配置为准。

## 9. 状态话题

### 9.1 标准关节状态

| 话题 | 类型 | 来源 | 用途 |
|---|---|---|---|
| `/joint_states` | `sensor_msgs/msg/JointState` | `joint_state_broadcaster` | RViz、TF、MoveIt 的主要状态源 |
| `/elfin_sdk/joint_states` | `sensor_msgs/msg/JointState` | ros2_control硬件插件的8893状态发布路径 | 直接观察控制器关节数据 |

关节位置、速度单位分别为 rad 和 rad/s。

```bash
ros2 topic echo /joint_states
ros2 topic hz /joint_states
```

### 9.2 实时机械臂状态

```text
/elfin_sdk/realtime_state
elfin_robot_msgs/msg/ElfinRealtimeState
```

字段：

| 字段 | 说明 | ROS 单位 |
|---|---|---|
| `joint_position_target` | 目标关节位置 | rad |
| `joint_velocity_target` | 目标关节速度 | rad/s |
| `joint_position_actual` | 实际关节位置 | rad |
| `joint_velocity_actual` | 实际关节速度 | rad/s |
| `joint_torque_actual` | 实际关节力矩 | 控制器协议单位 |
| `tcp_position_target` | 目标 TCP 位姿 | m、rad |
| `tcp_velocity_target` | 目标 TCP 速度 | m/s、rad/s |
| `tcp_position_actual` | 实际 TCP 位姿 | m、rad |
| `tcp_velocity_actual` | 实际 TCP 速度 | m/s、rad/s |
| `force_raw` | 原始力传感器数据 | 控制器协议单位 |
| `force_calibrated` | 标定后的力数据 | 控制器协议单位 |
| `speed_scaling` | 实际速度比例 | 0～1 |
| `controller_time_us` | 控制器时间 | us |
| `state_machine` | CDS 状态机数值 | 枚举值 |
| `force_control_state` | 力控状态 | 枚举值 |

### 9.3 机器人状态

```text
/elfin_sdk/robot_status
elfin_robot_msgs/msg/ElfinRobotStatus
```

包括 SDK 连接、运动、使能、故障、错误码、暂停、急停、安全防护、上电、到位、零力示教和
力控自由驱动状态。

该话题的运动、使能、暂停、到位、抱闸和错误信息来自10004 JSON推送，不再周期调用
`HRIF_ReadRobotState`。当前JSON数据没有独立的急停、安全防护和上电字段，因此
`emergency_stop`、`safeguard_stop`和`electrified`不再通过SDK补读，当前发布为`false`。

```bash
ros2 topic echo /elfin_sdk/robot_status
```

### 9.4 IO 状态

```text
/elfin_sdk/io_state
elfin_robot_msgs/msg/ElfinIOState
```

包括：

```text
digital_inputs
digital_outputs
configurable_inputs
configurable_outputs
analog_inputs
analog_output_modes
analog_outputs
```

### 9.5 末端IO状态

```text
/elfin_sdk/end_io_state
elfin_robot_msgs/msg/ElfinEndIOState
```

SDK节点直接解析10004 JSON的`EndIO`对象，不再调用`HRIF_ReadEndDI`、
`HRIF_ReadEndDO`或`HRIF_ReadEndAI`。协议固定包含4路DI、4路DO、4个末端按钮和2路AI。字段为：

| 字段 | 说明 |
|---|---|
| `digital_inputs` | 末端DI，数组下标为通道号 |
| `digital_outputs` | 末端DO，数组下标为通道号 |
| `buttons` | 4个末端按钮状态 |
| `buttons_enabled` | 末端按钮功能是否使能 |
| `analog_inputs` | 末端AI，数组下标为通道号 |

10004采用12字节小端帧头：4字节ASCII魔数`LTBR`、4字节`totalSize`和4字节
`dataSize`，随后是长度为`dataSize`的UTF-8 JSON正文，并满足
`totalSize = 12 + dataSize`。例如实机帧为`dataSize=2927`、
`totalSize=2939`。驱动从`StateAndError`、`ElectricBoxIO`、
`ElectricBoxAnalogIO`和`EndIO`对象读取状态及IO，同时处理TCP粘包/半包、魔数重同步、
接收超时、JSON解析错误和自动重连。`BrakeState`中0表示制动已释放，非0表示制动生效。

```bash
ros2 topic echo /elfin_sdk/end_io_state
ros2 topic hz /elfin_sdk/end_io_state
```

末端DO仍通过现有服务设置：

```bash
ros2 service call /elfin_sdk/set_digital_io \
  elfin_robot_msgs/srv/SetDigitalIO \
  "{domain: end_do, index: 0, value: true}"
```

## 10. MoveIt 与 RViz 状态链路

真实机械臂：

```text
8893
  → ElfinControllerHardware::read()
  → joint_state_broadcaster
  → /joint_states
  → robot_state_publisher
  → /tf
  → MoveIt / RViz
```

Gazebo：

```text
Gazebo physics
  → gazebo_ros2_control
  → joint_state_broadcaster
  → /joint_states
  → robot_state_publisher
  → /tf
  → MoveIt / RViz
```

MoveIt 规划轨迹执行链路：

```text
MoveIt
  → /elfin_arm_controller/follow_joint_trajectory
  → ros2_control
  → 8892 ServoJ（真实机械臂）
     或 Gazebo（仿真）
```

## 11. 推荐安全操作流程

### 从控制器控制切换到 ROS

1. 确认机械臂已使能且无报警；
2. 确认8892、8893、10003和10004可连接；
3. 退出 freedrive/force_freedrive；
4. 调用 `set_ros_control(true)`；
5. 确认 `elfin_arm_controller` 为 active；
6. 再从 MoveIt 执行轨迹。

### 开启零力示教

1. 调用 `set_freedrive(true)`；
2. 驱动先停止 ROS 运动控制器；
3. 8893和 `/joint_states` 继续更新；
4. 拖动机械臂时 RViz 同步更新；
5. 调用 `set_freedrive(false)` 退出；
6. 不会恢复旧轨迹；
7. 需要运动时显式调用 `set_ros_control(true)`。

不要把“关闭freedrive”和“恢复ROS控制”合并为一个隐式动作。分开操作可以让操作者先确认机械臂当前位置、人员已经离开工作空间且机器人状态正常，再把控制权交回MoveIt。

## 12. 常用诊断命令

```bash
nc -vz ROBOT_IP 8892
nc -vz ROBOT_IP 8893
nc -vz ROBOT_IP 10003
nc -vz ROBOT_IP 10004

ros2 control list_hardware_components -v
ros2 control list_controllers
ros2 control list_hardware_interfaces

ros2 topic hz /joint_states
ros2 topic echo /elfin_sdk/robot_status
ros2 service list | grep elfin_sdk
ros2 action list | grep follow_joint_trajectory
```

确认新增服务及校验开关：

```bash
ros2 service type /elfin_sdk/set_speed_ratio
ros2 launch elfin_robot_bringup elfin_control.launch.py --show-args | \
  grep enable_controller_validation
```

真实控制器启动时建议在日志中确认以下顺序：

```text
Preflight passed: ...
Elfin controller preflight passed; starting ros2_control
Successful 'activate' of hardware 'ElfinController'
8893 state stream is active
```

如果使用`enable_controller_validation:=false`，不会出现`Preflight passed`，但会明确输出校验已关闭。该选项只跳过机型和最低版本检查，不会关闭8893通信检查、ros2_control硬件激活检查或SDK连接检查。

诊断MoveIt到8892的实际发送内容：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=position \
  robot_ip:=10.20.215.133 command_log_throttle_ms:=0 \
  2>&1 | tee /tmp/elfin_position.log

grep -E '8892 TX|socket write|PushServoJ|SpeedJ|ERROR' \
  /tmp/elfin_position.log
```

日志中的`target_deg`是发送给8892的目标关节角，`actual_deg`是同一时刻8893反馈的实际关节角。`socket write: success`只表示数据写入TCP socket成功，不代表控制器已经接受并执行。

真实位置控制的预期控制器状态：

```text
joint_state_broadcaster  active
elfin_arm_controller     active
```

只读/控制器控制的预期状态：

```text
joint_state_broadcaster  active
elfin_arm_controller     inactive
```

## 13. 8893断线判断与自动恢复

8893按控制周期主动推送状态。驱动采用两级超时，短暂网络或调度抖动不会让
ros2_control硬件退出`active`：

- `state_stale_timeout_ms`（默认100 ms）：没有收到完整帧时标记状态陈旧并禁止8892写指令，同时保留硬件生命周期和最后一次有效状态。
- `state_disconnect_timeout_ms`（默认1000 ms）：持续没有完整帧达到该阈值后关闭并重连8893。

重新收到有效帧后，驱动先把命令接口同步到实际关节位置，再恢复状态更新。
TCP连接同时启用keepalive。正常关闭、连接reset或非法帧会立即进入重连；普通
`EAGAIN/EWOULDBLOCK`只表示本次接收超时，驱动会保留已经收到的半帧，不会立即误判断线。
两个阈值可以在启动时覆盖：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=controller \
  robot_ip:=10.20.215.133 \
  state_stale_timeout_ms:=100 state_disconnect_timeout_ms:=1000
```

## 14. MoveIt随机关节轨迹循环测试

`moveit_random_joint_test.py`通过`/move_action`随机生成关节目标，并让MoveIt完成规划和执行。默认每轮10组并持续循环，按`Ctrl+C`停止。

为降低真实机器人测试风险，随机目标以脚本启动时的六轴实际位置为固定中心，每个关节默认只在±0.15 rad范围内采样，而不是在完整关节范围内随机跳转。MoveIt仍会进行关节限制和碰撞检查。

```bash
ros2 run elfin_robot_bringup moveit_random_joint_test.py
```

常用参数：

```bash
ros2 run elfin_robot_bringup moveit_random_joint_test.py --ros-args \
  -p targets_per_cycle:=10 \
  -p loop:=true \
  -p max_offset:=0.15 \
  -p velocity_scaling:=0.1 \
  -p acceleration_scaling:=0.1 \
  -p pause_seconds:=1.0
```

只规划、不执行真实运动：

```bash
ros2 run elfin_robot_bringup moveit_random_joint_test.py --ros-args \
  -p execute:=false -p loop:=false
```

执行前必须确认机械臂已经使能、`ElfinController`和`elfin_arm_controller`均为active、工作空间无人且急停可用。脚本参数单位为rad和rad/s。
