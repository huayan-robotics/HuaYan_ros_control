# Elfin 控制面板

[English](README.md)

`elfin_basic_api` 提供连接控制器的 Elfin 操作界面。GUI 只通过 ROS 2
topic/service 与 `elfin_sdk_node` 通信，不直接加载 HRIF 动态库，也不直接连接
控制器端口。

## 环境要求

- Ubuntu 22.04、ROS 2 Humble
- `wxPython`（`sudo apt install python3-wxgtk4.0`）
- 控制器所需的 8892、8893、10003、10004 端口可访问
- `robot_model` 与控制器的机型别名一致，例如 `E05`

编译并加载工作空间：

```bash
cd ~/workspace/HuaYan_ros_control
colcon build --symlink-install
source /opt/ros/humble/setup.bash
source install/setup.bash
```

## 启动方法

本包同时保留原有 GUI 和新版控制器 GUI：

- 原有 GUI：`scripts/elfin_gui.py`，使用旧的 Basic API、MoveIt、TF 和轨迹控制接口。
- 新版 GUI：`scripts/elfin_gui_new.py`，使用 `elfin_sdk_node` 提供的控制器 SDK topic/service。

### 新版 GUI：完整启动

同时启动控制器 bringup、`elfin_sdk_node` 和新版 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

### 新版 GUI：只启动界面

如果控制器 bringup 和 `elfin_sdk_node` 已经在其他终端运行，只启动新版 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py
```

也可以直接运行新版脚本：

```bash
ros2 run elfin_basic_api elfin_gui_new.py
```

### 原有 GUI：启动界面

先按原功能包流程启动机器人驱动、`elfin_basic_api_node`、MoveIt/轨迹控制器和
相关 IO 服务，再运行原有 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py
```

该 launch 是原功能包自带的真实机器人 GUI 启动文件，设置
`use_fake_robot:=false` 和 `use_sim_time:=false`。也可只运行原脚本：

```bash
ros2 run elfin_basic_api elfin_gui.py
```

原有 GUI 依赖 `/joint_teleop`、`/cart_teleop`、`/stop_teleop`、
`/home_teleop`、`/read_di`、`/read_do`、`/write_do`、`/joint_states`、TF 和
`elfin_arm_controller/follow_joint_trajectory` 等旧接口。只运行 GUI 而未启动这些
后台节点时，窗口可以出现，但按钮和状态不会正常工作。

### 原有 fake GUI

保留原来的 fake launch，其启动的也是原有 `elfin_gui.py`：

```bash
ros2 launch elfin_basic_api fake_elfin_gui.launch.py
```

这个 launch 只给旧 GUI 设置 `use_fake_robot` 和 `use_sim_time` 参数，不会自动启动
Gazebo、MoveIt、旧 Basic API 或 IO 服务；这些依赖仍需按原仿真流程提前启动。

### 控制器校验

默认启用机型和版本校验。只有明确用于开发的控制器才应临时关闭校验：

```bash
ros2 launch elfin_basic_api elfin_gui_new.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103 \
  enable_controller_validation:=false
```

## 功能

- 实时显示控制器、使能、故障、运动、抱闸和 I/O 状态
- Servo On/Off、清错、软件急停和速度倍率
- 普通 Free Drive 和 ROS Control 模式切换
- 关节/笛卡尔按住点动
- 关节/笛卡尔目标、Home 和 TCP Z 轴对齐
- 当前 TCP、负载、碰撞安全等级和维护抱闸页面
- DI、CI、EndDI 只读反馈，以及可点击控制的 DO、CO、EndDO；输出颜色最终以
  控制器反馈为准

所有运动按钮均采用按住运行：松开鼠标即请求停止；keepalive 中断时，驱动
watchdog 也会停止运动。Brake 页面只在 Servo Off、机器人静止且无故障时允许
操作。松闸属于维护功能，操作前必须支撑机械臂，防止关节因重力下落。

界面中的 Free Drive 调用普通拖动接口 `HRIF_GrpOpenFreeDriver`，不是力控拖动。
该功能是否可用取决于控制器配置。力控拖动是独立 SDK 服务，并可能要求力传感器。

## 通信架构

```text
elfin_gui_new.py（wxPython/rclpy）
        | ROS 2 services/topics
elfin_sdk_node（C++/HRIF）
        | TCP 10003 指令 + TCP 10004 状态推送
Elfin 控制器
```

完整接口和实现设计见 [`elfin_gui.md`](elfin_gui.md)。
