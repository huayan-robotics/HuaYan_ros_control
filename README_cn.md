# Elfin Robot ROS 2

[English](README.md) · [API接口说明](docs/API_description_cn.md) ·
[控制器驱动使用手册](docs/controller_driver_guide_cn.md)

本仓库提供Elfin机器人在ROS 2 Humble下的模型、控制器驱动、`ros2_control`硬件接口、
MoveIt、Gazebo和操作界面。

## 1. 环境与编译

- Ubuntu 22.04
- ROS 2 Humble
- 支持机型：`E03`、`E05`、`E05-L`、`E10`、`E10-L`、`E15`

安装依赖：

```bash
sudo apt update
sudo apt install \
  ros-humble-controller-manager \
  ros-humble-ros2-control \
  ros-humble-ros2-controllers \
  ros-humble-joint-trajectory-controller \
  ros-humble-gazebo-ros2-control \
  ros-humble-moveit \
  python3-wxgtk4.0
```

编译并加载工作空间：

```bash
cd ~/elfin_ros2_ws
source /opt/ros/humble/setup.bash
colcon build --symlink-install
source install/setup.bash
```

确认包路径：

```bash
ros2 pkg prefix elfin_robot_bringup
```

## 2. 统一配置

统一启动入口：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py
```

日常参数集中在：

```text
elfin_robot_bringup/config/elfin_control.yaml
```

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

参数优先级为：命令行参数、YAML配置、launch回退值。命令行参数仅覆盖当前启动。

## 3. 真实控制器

启动前检查网络和端口：

```bash
ping ROBOT_IP
nc -vz ROBOT_IP 8892
nc -vz ROBOT_IP 8893
nc -vz ROBOT_IP 10003
nc -vz ROBOT_IP 10004
```

### 3.1 MoveIt位置控制

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=position \
  robot_ip:=192.168.56.103
```

该模式启动控制器通信、`ros2_control`、MoveIt和RViz。MoveIt通过
`/elfin_arm_controller/follow_joint_trajectory`执行轨迹。

### 3.2 控制器控制/状态监视

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=controller \
  robot_ip:=192.168.56.103
```

该模式持续发布真实状态，ROS运动控制器保持 `inactive`，8892不发送运动命令。

### 3.3 关节速度控制

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=controller control_mode:=velocity \
  robot_ip:=192.168.56.103
```

该模式通过 `elfin_velocity_controller`发送SpeedJ命令。MoveIt轨迹执行使用
`control_mode:=position`。

## 4. GUI

新版GUI完整启动：

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

驱动和SDK节点已经运行时仅启动新版GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

原有GUI：

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py
```

原有fake GUI：

```bash
ros2 launch elfin_basic_api fake_elfin_gui.launch.py
```

旧GUI launch仅启动界面，旧Basic API、MoveIt、TF、轨迹控制器和IO服务由原有流程启动。

## 5. Gazebo仿真

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=gazebo
```

该模式启动Gazebo、`gazebo_ros2_control`、状态广播器、轨迹控制器、MoveIt和RViz，
不连接真实控制器端口。

旧EtherCAT入口：

```bash
ros2 launch elfin_robot_bringup elfin_control.launch.py \
  robot_model:=E05 hardware_type:=ethercat
```

## 6. 启动参数

| 参数 | launch回退值 | 说明 |
|---|---:|---|
| `config_file` | 包内统一YAML | 统一配置文件绝对路径 |
| `robot_model` | `E05` | 控制器机型别名 |
| `hardware_type` | `gazebo` | `controller`、`gazebo`、`ethercat`或`fake` |
| `robot_ip` | `10.20.200.3` | 真实控制器IP |
| `control_mode` | `position` | `position`、`velocity`或`controller` |
| `update_rate` | `1000` | 1 ms控制器为1000，4 ms控制器为250 |
| `enable_controller_validation` | `true` | 校验机型和最低版本 |
| `servo_gain` | `8000` | 8892 StartServo增益 |
| `lookahead_time` | `0.004` | ServoJ前瞻时间，秒 |
| `servo_restart_idle_ms` | `100` | Servo流空闲阈值，ms |
| `state_stale_timeout_ms` | `100` | 8893状态陈旧阈值，ms |
| `state_disconnect_timeout_ms` | `1000` | 8893重连阈值，ms |
| `pushed_state_port` | `10004` | JSON状态推送端口 |
| `status_publish_rate` | `10.0` | 机器人状态发布频率，Hz |
| `io_publish_rate` | `5.0` | IO状态发布频率，Hz |

完整参数表见[控制器驱动使用手册](docs/controller_driver_guide_cn.md)。

## 7. 机型、版本与周期限制

`robot_model`使用控制器 `typealias`：`E03`、`E05`、`E05-L`、`E10`、`E10-L`、
`E15`。

启用控制器校验后：

- 控制器机型必须与 `robot_model`一致；
- 10003原始 `ReadVersion`响应最后的 `HR...`产品版本不得低于 `6.5.20d`；
- 机型、版本或SDK读取校验失败时不启动 `ros2_control`。

SDK返回的七段数字（例如 `20260724.31340.0.8228.1315.107.0`）是内部组件版本，仅用于
诊断日志，不参与产品版本比较。

控制频率必须与8893数据包中的 `cycle_time`一致：

| 控制器周期 | `update_rate` |
|---|---:|
| 1 ms | 1000 Hz |
| 4 ms | 250 Hz |

周期不一致时硬件接口拒绝激活。SDK服务、状态话题、单位和通信链路见
[API接口说明](docs/API_description_cn.md)。
