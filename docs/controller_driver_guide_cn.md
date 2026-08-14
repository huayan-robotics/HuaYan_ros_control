# Elfin ROS 2 控制器驱动使用手册

本文档说明工作空间编译、真实控制器与仿真启动、启动参数和控制器版本限制。
SDK服务、状态话题、GUI功能及通信实现统一见
[`API_description_cn.md`](API_description_cn.md)。

## 1. 环境与编译

适用 Ubuntu 22.04、ROS 2 Humble。支持 `E03`、`E05`、`E05-L`、`E10`、
`E10-L`、`E15`。

```bash
cd ~/elfin_ros2_ws
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

确认加载的是本工作空间：

```bash
ros2 pkg prefix elfin_robot_bringup
```

预期路径类似：

```text
/home/cjw/elfin_ros2_ws/install/elfin_robot_bringup
```

仅重新编译本次相关包：

```bash
colcon build --symlink-install --packages-select \
  elfin_robot_msgs elfin_controller_driver elfin_robot_bringup elfin_basic_api
source install/setup.bash
```

## 2. 统一配置与入口

统一入口为：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py
```

日常配置集中在 `elfin_robot_bringup/config/elfin_control.yaml`：

```yaml
elfin_control:
  robot_model: E05
  hardware_type: controller
  robot_ip: 10.20.215.133
  control_mode: position
  update_rate: 1000
  enable_controller_validation: false

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

YAML 是正常使用时的唯一配置位置。命令行同名参数只覆盖本次启动，不修改YAML。
未知字段、空值或非法枚举会使launch直接报错。

## 3. 真实控制器启动

启动前确认网络和端口：

```bash
ping ROBOT_IP
nc -vz ROBOT_IP 8892
nc -vz ROBOT_IP 8893
nc -vz ROBOT_IP 10003
nc -vz ROBOT_IP 10004
```

### 3.1 MoveIt位置控制

推荐直接读取统一配置启动：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py
```

也可以临时覆盖：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=position \
  robot_ip:=192.168.56.103
```

该模式启动真实通信、`ros2_control`、MoveIt和RViz。MoveIt轨迹发送至
`/elfin_arm_controller/follow_joint_trajectory`。

### 3.2 控制器控制/只读状态

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=controller \
  robot_ip:=192.168.56.103
```

该模式持续读取真实状态并更新RViz，运动控制器保持 `inactive`，ROS不向8892发送运动
命令，适用于示教器、SDK控制和状态观察。

### 3.3 速度控制

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=velocity \
  robot_ip:=192.168.56.103
```

该模式启动关节速度控制器；当前MoveIt轨迹执行仍应使用 `position` 模式。

### 3.4 GUI启动

#### 新版GUI完整启动

同时启动控制器bringup、SDK节点和新版GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

bringup和SDK节点已运行时只启动GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

GUI-only不启动 `controller_manager`，因此没有 `update_rate` 参数。

#### 原有GUI

原有GUI和启动文件继续保留。先按原功能包流程启动旧Basic API、MoveIt、TF、轨迹
控制器和IO服务，再启动界面：

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py
```

也可以直接运行：

```bash
ros2 run elfin_basic_api elfin_gui.py
```

该launch只启动旧GUI，不会自动启动其后台依赖。

#### 原有fake GUI

```bash
ros2 launch elfin_basic_api fake_elfin_gui.launch.py
```

该launch只向旧GUI设置 `use_fake_robot:=true` 和 `use_sim_time:=true`，不会自动启动
Gazebo、MoveIt、旧Basic API或IO服务。

`elfin_basic_api.launch.py`是历史后台入口，只启动模型资源硬编码为Elfin10的旧
`elfin_basic_api_node`，不启动驱动、MoveIt、GUI或仿真，不能替代新版完整入口。

## 4. Gazebo仿真启动

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=gazebo
```

该模式启动Gazebo、`gazebo_ros2_control`、状态广播器、位置轨迹控制器、MoveIt和RViz，
不连接8892、8893、10003或10004。可将 `robot_model` 替换为任一受支持机型。

旧EtherCAT入口仍保留：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=ethercat
```

`hardware_type:=fake` 尚无完整统一bringup路径；无控制器测试请使用Gazebo。

## 5. 启动参数

| 参数 | launch回退值 | 说明 |
|---|---:|---|
| `config_file` | 包内 `elfin_control.yaml` | 统一配置文件绝对路径 |
| `robot_model` | `E05` | 控制器机型别名 |
| `hardware_type` | `gazebo` | `controller`、`gazebo`、`ethercat`或`fake` |
| `robot_ip` | `10.20.200.3` | 真实控制器IP；统一YAML可覆盖 |
| `control_mode` | `position` | `position`、`velocity`或`controller` |
| `update_rate` | `1000` | 1 ms控制器为1000，4 ms控制器为250 |
| `enable_controller_validation` | `true` | 启动前校验机型和最低版本 |
| `servo_gain` | `8000` | 8892 `StartServo`增益 |
| `lookahead_time` | `0.004` | ServoJ前瞻时间，秒 |
| `servo_restart_idle_ms` | `100` | 位置命令空闲后重启Servo流阈值，ms |
| `position_command_epsilon` | `1e-8` | 位置命令变化阈值，rad |
| `velocity_command_epsilon` | `1e-8` | 速度命令变化阈值，rad/s |
| `command_log_throttle_ms` | `100` | 8892命令日志间隔；负值关闭 |
| `state_stale_timeout_ms` | `100` | 8893状态陈旧阈值，ms |
| `state_disconnect_timeout_ms` | `1000` | 8893持续无帧后的重连阈值，ms |
| `pushed_state_port` | `10004` | JSON状态推送端口 |
| `pushed_state_socket_timeout_ms` | `100` | 10004单次接收超时，ms |
| `pushed_state_disconnect_timeout_ms` | `1000` | 10004持续无帧后的重连阈值，ms |
| `status_publish_rate` | `10.0` | 机器人状态话题频率，Hz |
| `io_publish_rate` | `5.0` | IO状态话题频率，Hz |

表中是launch回退值。实际启动优先级为：命令行覆盖 > YAML > launch回退值。IP、
硬件类型和校验开关以当前启动所使用的 `config_file` 为准。

## 6. 机型、版本与周期限制

`robot_model` 必须使用控制器 `typealias`：`E03`、`E05`、`E05-L`、`E10`、
`E10-L`或`E15`。

开启 `enable_controller_validation` 后，启动前通过SDK 10003：

1. 读取控制器机型并与 `robot_model` 比较；比较忽略大小写、下划线和连字符；
2. 读取控制器版本，最低要求为 `6.5.20d`；
3. 机型不匹配、版本过低、版本无法解析或SDK读取失败时不启动 `ros2_control`。

临时关闭校验：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller robot_ip:=192.168.56.103 \
  enable_controller_validation:=false
```

关闭该开关只跳过机型和最低版本校验，不跳过8893周期检查。

| 控制器版本/周期 | `update_rate` |
|---|---:|
| 版本名称不带 `4ms`，`cycle_time=1 ms` | `1000` Hz |
| 升级包名称带 `4ms`，`cycle_time=4 ms` | `250` Hz |

正常使用只修改统一YAML中的一处，也可用 `update_rate:=250` 临时覆盖。驱动连接8893后
读取首个有效包并校验周期；仅接受250或1000。配置不匹配或周期未知时输出
`Control frequency mismatch` 或 `Unsupported 8893 cycle_time` 并拒绝激活。
