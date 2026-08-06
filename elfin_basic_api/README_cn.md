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

同时启动控制器驱动和 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py \
  robot_model:=E05 robot_ip:=192.168.56.103
```

如果控制器 bringup 和 `elfin_sdk_node` 已经启动，只启动 GUI：

```bash
ros2 launch elfin_basic_api elfin_gui_only.launch.py
```

默认启用机型和版本校验。只有明确用于开发的控制器才应临时关闭校验：

```bash
ros2 launch elfin_basic_api elfin_gui.launch.py \
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
- 控制器 DI/DO、CI/CO、EndDI/EndDO 反馈

所有运动按钮均采用按住运行：松开鼠标即请求停止；keepalive 中断时，驱动
watchdog 也会停止运动。Brake 页面只在 Servo Off、机器人静止且无故障时允许
操作。松闸属于维护功能，操作前必须支撑机械臂，防止关节因重力下落。

界面中的 Free Drive 调用普通拖动接口 `HRIF_GrpOpenFreeDriver`，不是力控拖动。
该功能是否可用取决于控制器配置。力控拖动是独立 SDK 服务，并可能要求力传感器。

## 通信架构

```text
elfin_gui.py（wxPython/rclpy）
        | ROS 2 services/topics
elfin_sdk_node（C++/HRIF）
        | TCP 10003 指令 + TCP 10004 状态推送
Elfin 控制器
```

完整接口和实现设计见 [`elfin_gui.md`](elfin_gui.md)。
