#!/usr/bin/env python3
"""Elfin operator panel.

The Brake page communicates only through ROS 2.  It never loads the controller
SDK directly and never changes controller power or safety-guard state.
"""

import math
import threading
import time
import wx

import rclpy
from rclpy.executors import MultiThreadedExecutor
from rclpy.node import Node

from std_srvs.srv import SetBool, Trigger

from elfin_robot_msgs.msg import (
    ElfinBrakeState,
    ElfinEndIOState,
    ElfinIOState,
    ElfinRealtimeState,
    ElfinRobotStatus,
)
from elfin_robot_msgs.srv import (
    GetInt32,
    GetPayload,
    GetPose,
    Jog,
    MoveTarget,
    SetBrake,
    SetFloat64,
    SetInt16,
    SetPayload,
    SetPose,
)


BLUE = wx.Colour(49, 132, 214)
LIGHT_BLUE = wx.Colour(219, 238, 255)
PALE_BLUE = wx.Colour(237, 247, 255)
RED = wx.Colour(238, 63, 69)
YELLOW = wx.Colour(255, 211, 57)
GREY = wx.Colour(202, 207, 213)
WHITE = wx.Colour(255, 255, 255)
BLACK = wx.Colour(30, 30, 30)


class GuiRosBridge(Node):
    """ROS callbacks are forwarded to wx; no wx widget is touched here."""

    def __init__(
        self, status_callback, brake_callback, realtime_callback, io_callback, end_io_callback
    ):
        super().__init__("elfin_gui")
        self.status_callback = status_callback
        self.brake_callback = brake_callback
        self.realtime_callback = realtime_callback
        self.io_callback = io_callback
        self.end_io_callback = end_io_callback
        self.brake_client = self.create_client(SetBrake, "/elfin_sdk/set_brake")
        self.enable_client = self.create_client(SetBool, "/elfin_sdk/set_enabled")
        self.reset_client = self.create_client(Trigger, "/elfin_sdk/reset")
        self.stop_client = self.create_client(Trigger, "/elfin_sdk/stop")
        self.freedrive_client = self.create_client(SetBool, "/elfin_sdk/set_freedrive")
        self.ros_control_client = self.create_client(SetBool, "/elfin_sdk/set_ros_control")
        self.speed_client = self.create_client(SetFloat64, "/elfin_sdk/set_speed_ratio")
        self.get_tcp_client = self.create_client(GetPose, "/elfin_sdk/get_tcp")
        self.set_tcp_client = self.create_client(SetPose, "/elfin_sdk/set_tcp")
        self.get_safety_client = self.create_client(
            GetInt32, "/elfin_sdk/get_collision_level"
        )
        self.set_safety_client = self.create_client(
            SetInt16, "/elfin_sdk/set_collision_level"
        )
        self.get_payload_client = self.create_client(GetPayload, "/elfin_sdk/get_payload")
        self.set_payload_client = self.create_client(SetPayload, "/elfin_sdk/set_payload")
        self.jog_client = self.create_client(Jog, "/elfin_sdk/jog")
        self.move_target_client = self.create_client(MoveTarget, "/elfin_sdk/move_target")
        self.create_subscription(
            ElfinRobotStatus, "/elfin_sdk/robot_status", self._on_status, 10
        )
        self.create_subscription(
            ElfinBrakeState, "/elfin_sdk/brake_state", self._on_brake, 10
        )
        self.create_subscription(
            ElfinRealtimeState, "/elfin_sdk/realtime_state", self._on_realtime, 10
        )
        self.create_subscription(ElfinIOState, "/elfin_sdk/io_state", self._on_io, 10)
        self.create_subscription(
            ElfinEndIOState, "/elfin_sdk/end_io_state", self._on_end_io, 10
        )

    def _on_status(self, message):
        wx.CallAfter(self.status_callback, message)

    def _on_brake(self, message):
        wx.CallAfter(self.brake_callback, message)

    def _on_realtime(self, message):
        wx.CallAfter(self.realtime_callback, message)

    def _on_io(self, message):
        wx.CallAfter(self.io_callback, message)

    def _on_end_io(self, message):
        wx.CallAfter(self.end_io_callback, message)

    @staticmethod
    def _request(client, request, operation, callback):
        if not client.service_is_ready():
            wx.CallAfter(callback, False, f"{operation} service is unavailable", None)
            return
        future = client.call_async(request)
        state = {"finished": False}

        def finish(success, detail, response):
            if state["finished"]:
                return
            state["finished"] = True
            if timeout.IsRunning():
                timeout.Stop()
            callback(success, detail, response)

        timeout = wx.CallLater(
            2000,
            finish,
            False,
            f"{operation} timed out; elfin_sdk may have stopped",
            None,
        )

        def complete(result_future):
            try:
                response = result_future.result()
                wx.CallAfter(finish, response.success, response.message, response)
            except Exception as error:  # rclpy transport failure
                wx.CallAfter(finish, False, str(error), None)

        future.add_done_callback(complete)

    def set_enabled(self, enabled, callback):
        request = SetBool.Request()
        request.data = enabled
        self._request(self.enable_client, request, "Servo", callback)

    def reset(self, callback):
        self._request(self.reset_client, Trigger.Request(), "Clear Fault", callback)

    def stop(self, callback):
        self._request(self.stop_client, Trigger.Request(), "Stop", callback)

    def set_freedrive(self, enabled, callback):
        request = SetBool.Request()
        request.data = enabled
        self._request(self.freedrive_client, request, "Free Drive", callback)

    def set_ros_control(self, enabled, callback):
        request = SetBool.Request()
        request.data = enabled
        self._request(self.ros_control_client, request, "ROS Control", callback)

    def set_speed(self, ratio, callback):
        request = SetFloat64.Request()
        request.data = ratio
        self._request(self.speed_client, request, "Velocity Scaling", callback)

    def get_tcp(self, callback):
        self._request(self.get_tcp_client, GetPose.Request(), "Get TCP", callback)

    def set_tcp(self, pose, callback):
        request = SetPose.Request()
        request.pose = pose
        self._request(self.set_tcp_client, request, "Set TCP", callback)

    def get_safety(self, callback):
        self._request(self.get_safety_client, GetInt32.Request(), "Get Safety Level", callback)

    def set_safety(self, level, callback):
        request = SetInt16.Request()
        request.data = level
        self._request(self.set_safety_client, request, "Set Safety Level", callback)

    def get_payload(self, callback):
        self._request(self.get_payload_client, GetPayload.Request(), "Get Payload", callback)

    def set_payload(self, mass, cog, callback):
        request = SetPayload.Request()
        request.mass = mass
        request.center_of_gravity = cog
        request.option = 1
        self._request(self.set_payload_client, request, "Set Payload", callback)

    def jog(self, mode, axis, direction, action, callback):
        request = Jog.Request()
        request.mode = mode
        request.axis = axis
        request.direction = direction
        request.action = action
        self._request(self.jog_client, request, "Jog", callback)

    def move_target(self, mode, target, action, hold_required, callback):
        request = MoveTarget.Request()
        request.mode = mode
        request.target = target
        request.velocity = 10.0
        request.acceleration = 20.0
        request.blend_radius = 0.0
        request.action = action
        request.hold_required = hold_required
        self._request(self.move_target_client, request, "Move Target", callback)

    def set_brake(self, axis, release, callback):
        if not self.brake_client.service_is_ready():
            wx.CallAfter(callback, False, "Brake service /elfin_sdk/set_brake is unavailable")
            return
        request = SetBrake.Request()
        request.axis = axis
        request.release = release
        self._request(
            self.brake_client,
            request,
            "Brake",
            lambda success, message, _response: callback(success, message),
        )


def make_button(parent, label, handler=None, colour=WHITE, size=(-1, 44)):
    button = wx.Button(parent, label=label, size=size)
    button.SetBackgroundColour(colour)
    if handler:
        button.Bind(wx.EVT_BUTTON, handler)
    return button


def make_text(parent, value="0.000", readonly=False, size=(100, 36)):
    style = wx.TE_CENTER
    if readonly:
        style |= wx.TE_READONLY
    control = wx.TextCtrl(parent, value=value, size=size, style=style)
    control.SetBackgroundColour(PALE_BLUE if readonly else WHITE)
    return control


class HeaderBar(wx.Panel):
    def __init__(self, parent):
        super().__init__(parent)
        self.SetBackgroundColour(WHITE)
        grid = wx.FlexGridSizer(1, 7, 2, 2)
        # The product title and IP address need more room than the state cells.
        for column, proportion in enumerate((2, 1, 2, 1, 1, 1, 1)):
            grid.AddGrowableCol(column, proportion)
        self.cells = {}
        values = (
            ("title", "ELFIN ROBOT\nCONTROL", BLUE),
            ("model", "Model: E05", BLUE),
            ("ip", "IP: 192.168.56.103", BLUE),
            ("sdk", "SDK Connected", BLUE),
            ("servo", "Servo Off", BLUE),
            ("fault", "No Fault", BLUE),
        )
        for key, label, colour in values:
            # Keep the colour on a fixed-size panel.  On GTK a StaticText can
            # shrink to its new label after SetLabel(), exposing white space.
            cell = wx.Panel(self)
            cell.SetBackgroundColour(colour)
            cell_sizer = wx.BoxSizer(wx.VERTICAL)
            text = wx.StaticText(cell, label=label, style=wx.ALIGN_CENTER)
            text.SetForegroundColour(WHITE)
            text.SetBackgroundColour(colour)
            font = text.GetFont()
            font.SetPointSize(10 if key == "title" else 11)
            font.SetWeight(wx.FONTWEIGHT_BOLD)
            text.SetFont(font)
            cell_sizer.AddStretchSpacer()
            cell_sizer.Add(text, 0, wx.ALIGN_CENTER | wx.LEFT | wx.RIGHT, 3)
            cell_sizer.AddStretchSpacer()
            cell.SetSizer(cell_sizer)
            grid.Add(cell, 1, wx.EXPAND)
            self.cells[key] = text
        self.stop = make_button(self, "STOP", colour=RED)
        self.stop.SetForegroundColour(WHITE)
        self.stop.Bind(wx.EVT_BUTTON, lambda _event: self.GetTopLevelParent().emergency_stop())
        grid.Add(self.stop, 1, wx.EXPAND)
        self.SetSizer(grid)
        self.SetMinSize((-1, 68))

    def set_servo(self, enabled):
        self.cells["servo"].SetLabel("Servo On" if enabled else "Servo Off")

    def set_fault(self, faulted):
        self.cells["fault"].SetLabel("Fault" if faulted else "No Fault")
        # Status cells keep the same blue appearance; only their text changes.

    def set_sdk_connected(self, connected):
        self.cells["sdk"].SetLabel("SDK Connected" if connected else "SDK Disconnected")

    def show_stop_result(self, label):
        self.stop.SetLabel(label)
        self.stop.SetBackgroundColour(RED)
        self.stop.Refresh()


class JogRow(wx.Panel):
    def __init__(self, parent, name, unit, frame, mode, axis):
        super().__init__(parent)
        self.name = name
        self.unit = unit
        self.frame = frame
        self.mode = mode
        self.axis = axis
        row = wx.BoxSizer(wx.HORIZONTAL)

        label = wx.StaticText(self, label=name, size=(38, -1), style=wx.ALIGN_CENTER)
        font = label.GetFont()
        font.SetWeight(wx.FONTWEIGHT_BOLD)
        label.SetFont(font)
        row.Add(label, 0, wx.ALIGN_CENTER_VERTICAL | wx.RIGHT, 5)
        self.actual = make_text(self, readonly=True, size=(105, 36))
        row.Add(self.actual, 0, wx.RIGHT, 5)
        unit_label = wx.StaticText(self, label=unit, size=(35, -1))
        row.Add(unit_label, 0, wx.ALIGN_CENTER_VERTICAL | wx.RIGHT, 8)

        minus = make_button(self, "−", colour=BLUE, size=(42, 36))
        minus.SetForegroundColour(WHITE)
        self.target = make_text(self, size=(90, 36))
        plus = make_button(self, "+", colour=BLUE, size=(42, 36))
        plus.SetForegroundColour(WHITE)
        row.Add(minus, 0, wx.RIGHT, 5)
        row.Add(self.target, 0, wx.RIGHT, 5)
        row.Add(plus, 0)
        self.SetSizer(row)

        self._bind_hold(minus, -1)
        self._bind_hold(plus, 1)

    def _bind_hold(self, button, direction):
        def start(event):
            self.frame.start_jog(self.mode, self.axis, direction)
            event.Skip()

        def stop(event):
            self.frame.stop_jog()
            event.Skip()

        button.Bind(wx.EVT_LEFT_DOWN, start)
        button.Bind(wx.EVT_LEFT_UP, stop)


class MainPage(wx.ScrolledWindow):
    def __init__(self, parent, frame):
        super().__init__(parent, style=wx.VSCROLL)
        self.frame = frame
        self.SetScrollRate(0, 12)
        root = wx.BoxSizer(wx.VERTICAL)
        root.AddSpacer(14)

        headings = wx.BoxSizer(wx.HORIZONTAL)
        for title in ("JOINT JOG", "CARTESIAN JOG"):
            text = wx.StaticText(self, label=title, style=wx.ALIGN_CENTER)
            font = text.GetFont()
            font.SetPointSize(14)
            font.SetWeight(wx.FONTWEIGHT_BOLD)
            text.SetFont(font)
            headings.Add(text, 1, wx.EXPAND)
        root.Add(headings, 0, wx.EXPAND | wx.LEFT | wx.RIGHT, 20)

        jog_area = wx.BoxSizer(wx.HORIZONTAL)
        joint_box = wx.BoxSizer(wx.VERTICAL)
        cart_box = wx.BoxSizer(wx.VERTICAL)
        self.joint_rows = []
        self.cart_rows = []
        for i in range(1, 7):
            jog_row = JogRow(self, f"J{i}", "°", frame, Jog.Request.MODE_JOINT, i - 1)
            self.joint_rows.append(jog_row)
            joint_box.Add(jog_row, 0, wx.EXPAND | wx.ALL, 2)
        for axis, (name, unit) in enumerate((("X", "mm"), ("Y", "mm"), ("Z", "mm"),
                                             ("RX", "°"), ("RY", "°"), ("RZ", "°"))):
            jog_row = JogRow(self, name, unit, frame, Jog.Request.MODE_CARTESIAN, axis)
            self.cart_rows.append(jog_row)
            cart_box.Add(jog_row, 0, wx.EXPAND | wx.ALL, 2)
        jog_area.Add(joint_box, 1, wx.EXPAND | wx.RIGHT, 15)
        jog_area.Add(cart_box, 1, wx.EXPAND | wx.LEFT, 15)
        root.Add(jog_area, 0, wx.EXPAND | wx.LEFT | wx.RIGHT | wx.TOP | wx.BOTTOM, 16)

        quick = wx.BoxSizer(wx.HORIZONTAL)
        home = make_button(self, "Home", colour=BLUE)
        self.align_button = make_button(self, "Z-axis Alignment", colour=BLUE)
        synchronize = make_button(
            self,
            "Synchronize",
            lambda _e: self.synchronize_targets(),
            BLUE,
        )
        quick.Add(home, 1, wx.RIGHT, 12)
        quick.Add(self.align_button, 1, wx.RIGHT, 12)
        quick.Add(synchronize, 1, wx.RIGHT, 12)
        hold = make_button(self, "Hold-to-run to Target", colour=BLUE)

        def start_hold(event):
            frame.start_hold_target(self.target_mode)
            event.Skip()

        def stop_hold(event):
            frame.stop_target()
            event.Skip()

        hold.Bind(wx.EVT_LEFT_DOWN, start_hold)
        hold.Bind(wx.EVT_LEFT_UP, stop_hold)
        self._bind_target_hold(home, MoveTarget.Request.MODE_HOME)
        self._bind_target_hold(self.align_button, MoveTarget.Request.MODE_ALIGN_Z)
        quick.Add(hold, 1)
        for child in quick.GetChildren():
            window = child.GetWindow()
            if window:
                window.SetForegroundColour(WHITE)
        root.Add(quick, 0, wx.EXPAND | wx.LEFT | wx.RIGHT | wx.BOTTOM, 20)

        speed = wx.BoxSizer(wx.HORIZONTAL)
        speed.Add(
            wx.StaticText(self, label="Velocity Scaling"),
            0,
            wx.ALIGN_CENTER_VERTICAL | wx.RIGHT,
            15,
        )
        self.speed_slider = wx.Slider(self, value=20, minValue=1, maxValue=100)
        self.speed_value = wx.StaticText(self, label="20 %", size=(65, -1), style=wx.ALIGN_CENTER)
        self.speed_slider.Bind(wx.EVT_SLIDER, self.on_speed)
        self.speed_request = None
        speed.Add(self.speed_slider, 1, wx.ALIGN_CENTER_VERTICAL | wx.RIGHT, 15)
        speed.Add(self.speed_value, 0, wx.ALIGN_CENTER_VERTICAL)
        root.Add(speed, 0, wx.EXPAND | wx.LEFT | wx.RIGHT | wx.BOTTOM, 22)

        # Direct actions stay on the first row; these buttons do not change page.
        controls = wx.GridSizer(1, 5, 14, 14)
        controls.Add(
            make_button(self, "Servo On", lambda _e: frame.set_servo(True), LIGHT_BLUE),
            1,
            wx.EXPAND,
        )
        controls.Add(
            make_button(self, "Servo Off", lambda _e: frame.set_servo(False), LIGHT_BLUE),
            1,
            wx.EXPAND,
        )
        controls.Add(
            make_button(self, "Clear Fault", lambda _e: frame.clear_fault(), LIGHT_BLUE),
            1,
            wx.EXPAND,
        )
        self.free_drive = make_button(self, "Free Drive", self.toggle_free_drive, LIGHT_BLUE)
        controls.Add(self.free_drive, 1, wx.EXPAND)
        self.ros = make_button(self, "ROS Activate", self.toggle_ros, LIGHT_BLUE)
        controls.Add(self.ros, 1, wx.EXPAND)
        root.Add(controls, 0, wx.EXPAND | wx.LEFT | wx.RIGHT | wx.BOTTOM, 22)

        # Page navigation occupies a separate five-button row.
        navigation = wx.GridSizer(1, 5, 14, 14)
        for label, page in (("Safety Level", "safety"), ("Set I/O", "io"),
                            ("Payload", "payload"), ("TCP", "tcp"), ("Brake", "brake")):
            navigation.Add(
                make_button(self, label, lambda _e, p=page: frame.show_page(p), BLUE),
                1,
                wx.EXPAND,
            )
        for child in navigation.GetChildren():
            child.GetWindow().SetForegroundColour(WHITE)
        root.Add(navigation, 0, wx.EXPAND | wx.LEFT | wx.RIGHT | wx.BOTTOM, 18)
        self.SetSizer(root)

        self.free_drive_enabled = False
        self.free_drive_pending = None
        self.free_drive_failure_until = 0.0
        self.free_drive_failure_label = ""
        self.ros_active = False
        self.target_mode = MoveTarget.Request.MODE_JOINT
        for row in self.joint_rows:
            row.target.Bind(
                wx.EVT_TEXT,
                lambda _e: self._set_target_mode(MoveTarget.Request.MODE_JOINT),
            )
        for row in self.cart_rows:
            row.target.Bind(
                wx.EVT_TEXT,
                lambda _e: self._set_target_mode(MoveTarget.Request.MODE_CARTESIAN),
            )

    def _bind_target_hold(self, button, mode):
        def start(event):
            self.frame.start_special_target(mode)
            event.Skip()

        def stop(event):
            self.frame.stop_target()
            event.Skip()

        button.Bind(wx.EVT_LEFT_DOWN, start)
        button.Bind(wx.EVT_LEFT_UP, stop)

    def _set_target_mode(self, mode):
        self.target_mode = mode

    def synchronize_targets(self):
        for row in self.joint_rows + self.cart_rows:
            row.target.SetValue(row.actual.GetValue())

    def target_values(self, mode):
        rows = self.joint_rows if mode == MoveTarget.Request.MODE_JOINT else self.cart_rows
        return [float(row.target.GetValue()) for row in rows]

    def on_speed(self, _event):
        value = self.speed_slider.GetValue()
        if self.speed_request is not None and self.speed_request.IsRunning():
            self.speed_request.Stop()
        self.speed_request = wx.CallLater(50, self.frame.set_speed, value / 100.0)

    def toggle_free_drive(self, _event):
        self.frame.set_freedrive(not self.free_drive_enabled)

    def toggle_ros(self, _event):
        self.frame.set_ros_control(not self.ros_active)

    def update_robot_status(self, message):
        self.free_drive_enabled = message.freedrive
        self.ros_active = message.ros_control_active
        preserve_feedback = False
        if self.free_drive_pending is not None:
            if message.freedrive == self.free_drive_pending:
                self.free_drive_pending = None
            else:
                self.free_drive.Enable(False)
                preserve_feedback = True
        if time.monotonic() < self.free_drive_failure_until:
            self.free_drive.SetLabel(self.free_drive_failure_label)
            self.free_drive.Enable(True)
            preserve_feedback = True
        if not preserve_feedback:
            self.free_drive.SetLabel("Exit Free Drive" if message.freedrive else "Free Drive")
            self.free_drive.SetBackgroundColour(YELLOW if message.freedrive else LIGHT_BLUE)
            self.free_drive.Enable(message.sdk_connected)
            self.free_drive.Refresh()
        self.ros.SetLabel("ROS Inactivate" if message.ros_control_active else "ROS Activate")

    def set_freedrive_pending(self, entering):
        self.free_drive_pending = entering
        self.free_drive.SetLabel("Entering..." if entering else "Exiting...")
        self.free_drive.Enable(False)

    def set_freedrive_failure(self, detail):
        self.free_drive_pending = None
        if "20606" in detail:
            label = "Free Drive Disabled"
        elif "40097" in detail:
            label = "No Force Sensor"
        else:
            label = "Free Drive Failed"
        self.free_drive_failure_label = label
        self.free_drive_failure_until = time.monotonic() + 5.0
        self.free_drive.SetLabel(label)
        self.free_drive.Enable(True)

    def set_alignment_state(self, label, active=False):
        self.align_button.SetLabel(label)
        self.align_button.SetBackgroundColour(YELLOW if active else BLUE)
        self.align_button.Refresh()

    def update_realtime(self, message):
        for row, value in zip(self.joint_rows, message.joint_position_actual):
            row.actual.SetValue(f"{math.degrees(value):.3f}")
        for index, (row, value) in enumerate(zip(self.cart_rows, message.tcp_position_actual)):
            displayed = value * 1000.0 if index < 3 else math.degrees(value)
            row.actual.SetValue(f"{displayed:.3f}")
        percentage = max(1, min(100, round(message.speed_scaling * 100.0)))
        if not self.speed_slider.HasCapture():
            self.speed_slider.SetValue(percentage)
        self.speed_value.SetLabel(f"{percentage} %")


class SubPage(wx.Panel):
    def __init__(self, parent, frame, title):
        super().__init__(parent)
        self.frame = frame
        self.root = wx.BoxSizer(wx.VERTICAL)
        top = wx.BoxSizer(wx.HORIZONTAL)
        top.Add(
            make_button(self, "←  Back", lambda _e: frame.show_page("main"), size=(115, 42)),
            0,
            wx.ALIGN_CENTER_VERTICAL,
        )
        heading = wx.StaticText(
            self,
            label=title,
            size=(-1, 58),
            style=wx.ALIGN_CENTER | wx.ST_NO_AUTORESIZE,
        )
        font = heading.GetFont()
        font.SetPointSize(20)
        font.SetWeight(wx.FONTWEIGHT_BOLD)
        heading.SetFont(font)
        top.Add(heading, 1, wx.EXPAND | wx.RIGHT, 115)
        self.root.Add(top, 0, wx.EXPAND | wx.LEFT | wx.RIGHT | wx.TOP, 20)
        self.SetSizer(self.root)

    def add_actions(self, save_handler, cancel_handler):
        actions = wx.BoxSizer(wx.HORIZONTAL)
        actions.AddStretchSpacer()
        save = make_button(self, "Save", save_handler, BLUE, (130, 45))
        save.SetForegroundColour(WHITE)
        actions.Add(save, 0, wx.RIGHT, 15)
        cancel = make_button(self, "Cancel", cancel_handler, WHITE, (130, 45))
        actions.Add(cancel, 0)
        self.root.Add(actions, 0, wx.EXPAND | wx.ALL, 25)
        return save, cancel


class SafetyPage(SubPage):
    def __init__(self, parent, frame):
        super().__init__(parent, frame, "Safety Level")
        self.saved_level = 0
        self.selected_level = 0
        self.buttons = []
        row = wx.BoxSizer(wx.HORIZONTAL)
        row.AddStretchSpacer()
        for level in range(6):
            button = make_button(
                self, str(level), lambda _e, n=level: self.select(n), size=(105, 72)
            )
            font = button.GetFont()
            font.SetPointSize(18)
            font.SetWeight(wx.FONTWEIGHT_BOLD)
            button.SetFont(font)
            self.buttons.append(button)
            row.Add(button, 0, wx.RIGHT, 8)
        row.AddStretchSpacer()
        self.root.AddStretchSpacer()
        self.root.Add(row, 0, wx.EXPAND | wx.ALL, 25)
        self.root.AddStretchSpacer()
        self.save_button, self.cancel_button = self.add_actions(self.save, self.cancel)
        self.select(0)

    def set_write_allowed(self, allowed):
        self.save_button.Enable(allowed)

    def set_level(self, level):
        self.saved_level = level
        self.select(level)

    def select(self, level):
        self.selected_level = level
        for index, button in enumerate(self.buttons):
            button.SetBackgroundColour(YELLOW if index <= level else WHITE)
            button.Refresh()
        self.frame.set_status(f"Safety level {level} selected (not saved)")

    def save(self, _event):
        self.frame.set_safety(self.selected_level)

    def cancel(self, _event):
        self.frame.read_safety()


class IoPage(SubPage):
    def __init__(self, parent, frame):
        super().__init__(parent, frame, "I/O Status")
        self.values = {}
        grid = wx.FlexGridSizer(6, 9, 9, 12)
        for prefix, count in (
            ("DI", 8), ("DO", 8), ("CI", 8), ("CO", 8), ("EndDI", 4), ("EndDO", 4)
        ):
            label = wx.StaticText(
                self,
                label=prefix,
                size=(72, -1),
                style=wx.ALIGN_RIGHT | wx.ST_NO_AUTORESIZE,
            )
            font = label.GetFont()
            font.SetWeight(wx.FONTWEIGHT_BOLD)
            label.SetFont(font)
            grid.Add(label, 0, wx.ALIGN_CENTER_VERTICAL)
            for index in range(8):
                if index < count:
                    name = f"{prefix}{index}"
                    button = make_button(self, name, size=(86, 44))
                    self.values[name] = button
                    grid.Add(button, 0, wx.EXPAND)
                else:
                    grid.AddSpacer(1)
        # Do not use vertical stretch spacers here: they can push the EndDO
        # row and action buttons below the visible area on a short display.
        legend = wx.StaticText(self, label="Yellow = ON    Grey = OFF    White = unavailable")
        self.live = wx.StaticText(self, label="Waiting for /elfin_sdk/io_state...")
        self.root.Add(grid, 0, wx.ALIGN_CENTER | wx.LEFT | wx.RIGHT | wx.TOP, 28)
        self.root.Add(legend, 0, wx.ALIGN_CENTER | wx.TOP, 15)
        self.root.Add(self.live, 0, wx.ALIGN_CENTER | wx.TOP, 8)

    def update_channels(self, prefix, values):
        for index in range(8):
            button = self.values.get(f"{prefix}{index}")
            if button is None:
                continue
            value = values[index] if index < len(values) else -1
            if value < 0:
                colour = WHITE
                state = "N/A"
            else:
                colour = YELLOW if value != 0 else GREY
                state = "ON" if value != 0 else "OFF"
            button.SetLabel(f"{prefix}{index}\n{state}")
            button.SetToolTip(f"10004 raw value: {value}")
            button.SetBackgroundColour(colour)
            button.Refresh()
        self.live.SetLabel(f"Live controller feedback: {time.strftime('%H:%M:%S')}")


class BrakeAxisControl(wx.Panel):
    """Brake tile with reliably centred text on GTK."""

    def __init__(self, parent, axis, handler):
        super().__init__(parent, size=(150, 120), style=wx.BORDER_SIMPLE)
        self.axis = axis
        self.handler = handler
        self.click_enabled = False
        self.pressed = False
        content = wx.BoxSizer(wx.VERTICAL)
        content.AddStretchSpacer()
        self.axis_text = wx.StaticText(
            self,
            label=f"Axis{axis}",
            size=(140, 26),
            style=wx.ALIGN_CENTER | wx.ST_NO_AUTORESIZE,
        )
        self.axis_text.SetMinSize((140, 26))
        axis_font = self.axis_text.GetFont()
        axis_font.SetPointSize(13)
        self.axis_text.SetFont(axis_font)
        self.state_text = wx.StaticText(
            self,
            label="Hold to Release",
            size=(140, 24),
            style=wx.ALIGN_CENTER | wx.ST_NO_AUTORESIZE,
        )
        self.state_text.SetMinSize((140, 24))
        content.Add(self.axis_text, 0, wx.EXPAND | wx.LEFT | wx.RIGHT, 5)
        content.AddSpacer(18)
        content.Add(self.state_text, 0, wx.EXPAND | wx.LEFT | wx.RIGHT, 5)
        content.AddStretchSpacer()
        self.SetSizer(content)
        for control in (self, self.axis_text, self.state_text):
            control.Bind(wx.EVT_LEFT_DOWN, self.on_press)
            control.Bind(wx.EVT_LEFT_UP, self.on_release)
        self.Bind(wx.EVT_MOUSE_CAPTURE_LOST, self.on_capture_lost)
        self.set_state(False, False)

    def on_press(self, event):
        if self.click_enabled and not self.pressed:
            self.pressed = True
            if not self.HasCapture():
                self.CaptureMouse()
            self.handler(True)
        event.Skip()

    def on_release(self, event):
        if self.pressed:
            self.pressed = False
            if self.HasCapture():
                self.ReleaseMouse()
            self.handler(False)
        event.Skip()

    def on_capture_lost(self, _event):
        if self.pressed:
            self.pressed = False
            self.handler(False)

    def set_state(self, released, change_allowed):
        self.click_enabled = change_allowed
        colour = GREY if released else BLUE
        self.SetBackgroundColour(colour)
        self.axis_text.SetBackgroundColour(colour)
        self.state_text.SetBackgroundColour(colour)
        if released:
            label = "Released"
        elif change_allowed:
            label = "Hold to Release"
        else:
            label = "Unavailable"
        self.state_text.SetLabel(label)
        self.SetCursor(wx.Cursor(wx.CURSOR_HAND if change_allowed else wx.CURSOR_ARROW))
        self.Layout()
        self.Refresh()


class BrakePage(SubPage):
    """Maintenance brake release; controller feedback is authoritative."""

    AVAILABLE_MESSAGE = (
        "Robot is Servo Off, stationary and fault-free. Hold an axis button to release "
        "its brake; releasing the button requests CloseBrake.\n"
        "⚠ Joints may drop under gravity after brake release. Please support the "
        "manipulator manually!"
    )
    UNAVAILABLE_MESSAGE = "Requires Servo Off, stopped and fault-free."

    def __init__(self, parent, frame):
        super().__init__(parent, frame, "Brake")
        self.operation_allowed = False
        self.released = [False] * 6

        self.message = wx.StaticText(
            self,
            label=self.AVAILABLE_MESSAGE,
            size=(820, -1),
            style=wx.ST_NO_AUTORESIZE,
        )
        message_font = self.message.GetFont()
        message_font.SetPointSize(11)
        self.message.SetFont(message_font)
        self.message.Wrap(820)
        self.root.Add(self.message, 0, wx.ALIGN_CENTER | wx.TOP, 55)

        axes = wx.BoxSizer(wx.VERTICAL)
        self.axis_buttons = []
        for row_index in range(2):
            row = wx.BoxSizer(wx.HORIZONTAL)
            for column in range(3):
                index = row_index * 3 + column
                button = BrakeAxisControl(
                    self,
                    index + 1,
                    lambda release, axis=index: self.request_axis(axis, release),
                )
                self.axis_buttons.append(button)
                row.Add(button, 0)
                if column < 2:
                    row.AddStretchSpacer()
            axes.Add(row, 0, wx.EXPAND)
            if row_index == 0:
                axes.AddSpacer(36)
        axes.SetMinSize((820, -1))
        self.root.Add(axes, 0, wx.ALIGN_CENTER | wx.TOP, 28)
        self.update_conditions(False)

    def update_conditions(self, operation_allowed):
        self.operation_allowed = operation_allowed
        self.message.SetLabel(
            self.AVAILABLE_MESSAGE if operation_allowed else self.UNAVAILABLE_MESSAGE
        )
        self.message.Wrap(820)
        for index, button in enumerate(self.axis_buttons):
            button.set_state(self.released[index], operation_allowed)
        self.Layout()

    def update_brakes(self, released):
        self.released = list(released[:6])
        for index, button in enumerate(self.axis_buttons):
            button.set_state(self.released[index], self.operation_allowed)

    def request_axis(self, axis, release):
        if release and not self.operation_allowed:
            return
        self.frame.request_brake(axis, release)


class FormPage(SubPage):
    def __init__(self, parent, frame, title, name, fields):
        super().__init__(parent, frame, title)
        self.controls = {}
        form = wx.FlexGridSizer(len(fields) + 1, 3, 18, 18)
        form.Add(wx.StaticText(self, label="Name"), 0, wx.ALIGN_CENTER_VERTICAL)
        fixed_name = make_text(self, name, readonly=True, size=(250, 38))
        form.Add(fixed_name, 0)
        form.AddSpacer(1)
        for key, unit in fields:
            form.Add(wx.StaticText(self, label=key), 0, wx.ALIGN_CENTER_VERTICAL)
            control = make_text(self, "0.000", size=(250, 38))
            self.controls[key] = control
            form.Add(control, 0)
            form.Add(wx.StaticText(self, label=unit), 0, wx.ALIGN_CENTER_VERTICAL)
        self.saved_values = {key: "0.000" for key in self.controls}
        self.root.AddStretchSpacer()
        self.root.Add(form, 0, wx.ALIGN_CENTER | wx.ALL, 30)
        self.root.AddStretchSpacer()
        self.save_button, self.cancel_button = self.add_actions(self.save, self.cancel)

    def save(self, _event):
        for key, control in self.controls.items():
            try:
                float(control.GetValue())
            except ValueError:
                wx.MessageBox(f"{key} must be a number.", "Invalid value", wx.OK | wx.ICON_WARNING)
                control.SetFocus()
                return
        self.saved_values = {key: control.GetValue() for key, control in self.controls.items()}
        self.frame.set_status(f"{self.__class__.__name__} settings saved locally")

    def cancel(self, _event):
        for key, value in self.saved_values.items():
            self.controls[key].SetValue(value)
        self.frame.set_status("Changes cancelled")


class TcpPage(FormPage):
    FIELD_ORDER = ("X", "Y", "Z", "RX", "RY", "RZ")

    def __init__(self, parent, frame):
        super().__init__(
            parent,
            frame,
            "TCP",
            "Current TCP",
            (("X", "mm"), ("Y", "mm"), ("Z", "mm"),
             ("RX", "°"), ("RY", "°"), ("RZ", "°")),
        )
        self.set_write_allowed(False)

    def set_write_allowed(self, allowed):
        for control in self.controls.values():
            control.SetEditable(allowed)
            control.SetBackgroundColour(WHITE if allowed else PALE_BLUE)
        self.save_button.Enable(allowed)

    def set_pose(self, pose):
        for name, value in zip(self.FIELD_ORDER, pose):
            self.controls[name].SetValue(f"{value:.3f}")
        self.saved_values = {
            name: self.controls[name].GetValue() for name in self.FIELD_ORDER
        }

    def save(self, _event):
        try:
            pose = [float(self.controls[name].GetValue()) for name in self.FIELD_ORDER]
        except ValueError:
            wx.MessageBox("TCP values must be numbers.", "Invalid TCP", wx.OK | wx.ICON_WARNING)
            return
        self.frame.set_tcp(pose)

    def cancel(self, _event):
        self.frame.read_tcp()


class PayloadPage(FormPage):
    FIELD_ORDER = ("Payload", "CX", "CY", "CZ")

    def __init__(self, parent, frame):
        super().__init__(parent, frame, "Payload", "Current Payload",
                         (("Payload", "kg"), ("CX", "mm"),
                          ("CY", "mm"), ("CZ", "mm")))
        self.max_payload = 0.0
        self.set_write_allowed(False)

    def set_write_allowed(self, allowed):
        for control in self.controls.values():
            control.SetEditable(allowed)
            control.SetBackgroundColour(WHITE if allowed else PALE_BLUE)
        self.save_button.Enable(allowed)

    def set_payload(self, mass, cog, max_payload):
        values = (mass, *cog)
        for name, value in zip(self.FIELD_ORDER, values):
            self.controls[name].SetValue(f"{value:.3f}")
        self.max_payload = max_payload

    def save(self, _event):
        try:
            values = [float(self.controls[name].GetValue()) for name in self.FIELD_ORDER]
        except ValueError:
            wx.MessageBox("Payload values must be numbers.", "Invalid Payload",
                          wx.OK | wx.ICON_WARNING)
            return
        self.frame.set_payload(values[0], values[1:])

    def cancel(self, _event):
        self.frame.read_payload()


class ElfinGuiFrame(wx.Frame):
    PAGE_ORDER = {
        "main": 0,
        "safety": 1,
        "io": 2,
        "payload": 3,
        "tcp": 4,
        "brake": 5,
    }

    def __init__(self):
        style = (wx.DEFAULT_FRAME_STYLE | wx.RESIZE_BORDER | wx.MINIMIZE_BOX |
                 wx.MAXIMIZE_BOX | wx.CLOSE_BOX)
        super().__init__(None, title="Elfin Robot Control", size=(1182, 820), style=style)
        self.SetMinSize((980, 720))
        panel = wx.Panel(self)
        root = wx.BoxSizer(wx.VERTICAL)
        self.header = HeaderBar(panel)
        root.Add(self.header, 0, wx.EXPAND)

        self.book = wx.Simplebook(panel)
        self.main_page = MainPage(self.book, self)
        self.brake_page = BrakePage(self.book, self)
        self.tcp_page = TcpPage(self.book, self)
        self.safety_page = SafetyPage(self.book, self)
        self.payload_page = PayloadPage(self.book, self)
        self.io_page = IoPage(self.book, self)
        pages = (
            self.main_page,
            self.safety_page,
            self.io_page,
            self.payload_page,
            self.tcp_page,
            self.brake_page,
        )
        for page in pages:
            self.book.AddPage(page, "")
        root.Add(self.book, 1, wx.EXPAND)
        panel.SetSizer(root)

        self.robot_status_received = False
        self.brake_status_received = False
        self.servo_enabled = False
        self.robot_moving = False
        self.robot_faulted = False
        self.sdk_connected = False
        self.active_jog = None
        self.jog_keepalive = None
        self.jog_request_pending = False
        self.jog_generation = 0
        self.target_keepalive = None
        self.target_request_pending = False
        self.target_generation = 0
        self.error_dialogs = set()
        self.last_status_time = None
        self.ros_bridge = GuiRosBridge(
            self.on_robot_status,
            self.on_brake_status,
            self.on_realtime_state,
            self.on_io_state,
            self.on_end_io_state,
        )
        self.ros_executor = MultiThreadedExecutor(num_threads=2)
        self.ros_executor.add_node(self.ros_bridge)
        self.ros_thread = threading.Thread(target=self.ros_executor.spin, daemon=True)
        self.ros_thread.start()
        self.state_watchdog = wx.Timer(self)
        self.Bind(wx.EVT_TIMER, self.check_state_freshness, self.state_watchdog)
        self.state_watchdog.Start(250)
        self.Bind(wx.EVT_CLOSE, self.on_close)
        self.Centre()

    def set_status(self, message):
        # Communication feedback will be designed separately.  The interface
        # The operator panel intentionally has no bottom status text.
        pass

    def show_page(self, name):
        if name == "brake":
            self.update_brake_availability()
        elif name == "tcp":
            self.read_tcp()
        elif name == "safety":
            self.read_safety()
        elif name == "payload":
            self.read_payload()
        self.book.SetSelection(self.PAGE_ORDER[name])

    def brake_operation_allowed(self):
        return (
            self.robot_status_received
            and self.brake_status_received
            and self.sdk_connected
            and not self.servo_enabled
            and not self.robot_moving
            and not self.robot_faulted
        )

    def update_brake_availability(self):
        self.brake_page.update_conditions(self.brake_operation_allowed())

    def on_robot_status(self, message):
        self.robot_status_received = True
        self.last_status_time = time.monotonic()
        self.sdk_connected = message.sdk_connected
        self.servo_enabled = message.enabled
        self.robot_moving = message.moving
        self.robot_faulted = message.error
        self.header.set_servo(message.enabled)
        self.header.set_fault(message.error)
        self.header.set_sdk_connected(message.sdk_connected)
        if message.emergency_stop or message.safeguard_stop:
            self.header.show_stop_result("E-STOP ACTIVE")
        elif self.header.stop.GetLabel() == "E-STOP ACTIVE":
            self.header.show_stop_result("STOP")
        self.main_page.update_robot_status(message)
        self.tcp_page.set_write_allowed(
            message.sdk_connected and not message.enabled and not message.moving
        )
        write_allowed = message.sdk_connected and not message.enabled and not message.moving
        self.safety_page.set_write_allowed(write_allowed)
        self.payload_page.set_write_allowed(write_allowed)
        self.update_brake_availability()

    def check_state_freshness(self, _event):
        if self.last_status_time is None or time.monotonic() - self.last_status_time <= 1.0:
            return
        self.last_status_time = None
        self.robot_status_received = False
        self.sdk_connected = False
        self.header.set_sdk_connected(False)
        self.cancel_motion_keepalives()
        self.tcp_page.set_write_allowed(False)
        self.safety_page.set_write_allowed(False)
        self.payload_page.set_write_allowed(False)
        self.update_brake_availability()

    def cancel_motion_keepalives(self):
        self.active_jog = None
        self.active_target = None
        self.jog_generation += 1
        self.target_generation += 1
        self.jog_request_pending = False
        self.target_request_pending = False
        for timer in (self.jog_keepalive, self.target_keepalive):
            if timer is not None and timer.IsRunning():
                timer.Stop()

    def on_brake_status(self, message):
        self.brake_status_received = True
        self.brake_page.update_brakes(message.released)
        self.update_brake_availability()

    def on_realtime_state(self, message):
        self.main_page.update_realtime(message)

    def on_io_state(self, message):
        self.io_page.update_channels("DI", message.digital_inputs)
        self.io_page.update_channels("DO", message.digital_outputs)
        self.io_page.update_channels("CI", message.configurable_inputs)
        self.io_page.update_channels("CO", message.configurable_outputs)

    def on_end_io_state(self, message):
        self.io_page.update_channels("EndDI", message.digital_inputs)
        self.io_page.update_channels("EndDO", message.digital_outputs)

    def show_service_error(self, operation, detail):
        dialog = wx.MessageDialog(
            self,
            f"{operation} failed:\n{detail}",
            "Controller command rejected",
            wx.OK | wx.ICON_ERROR,
        )
        self.error_dialogs.add(dialog)

        def close_dialog(event):
            self.error_dialogs.discard(dialog)
            dialog.Destroy()
            event.Skip(False)

        dialog.Bind(wx.EVT_BUTTON, close_dialog, id=wx.ID_OK)
        dialog.Bind(wx.EVT_CLOSE, close_dialog)
        dialog.Show()

    def command_result(self, operation, success, detail, _response=None):
        if not success:
            self.show_service_error(operation, detail)

    def request_brake(self, axis, release):
        # Opening is safety-gated. A release event must still attempt CloseBrake
        # even if status changes while the operator is holding the button.
        if release and not self.brake_operation_allowed():
            self.update_brake_availability()
            return
        self.ros_bridge.set_brake(
            axis,
            release,
            lambda success, detail: self.on_brake_response(axis, release, success, detail),
        )

    def on_brake_response(self, axis, release, success, detail):
        if success:
            return  # Wait for /elfin_sdk/brake_state; do not fake feedback.
        operation = "OpenBrake" if release else "CloseBrake"
        self.show_service_error(f"Axis{axis + 1} {operation}", detail)

    def set_servo(self, enabled):
        self.ros_bridge.set_enabled(
            enabled,
            lambda success, detail, response: self.command_result(
                "Servo On" if enabled else "Servo Off", success, detail, response
            ),
        )

    def clear_fault(self):
        self.ros_bridge.reset(
            lambda success, detail, response: self.command_result(
                "Clear Fault", success, detail, response
            )
        )

    def emergency_stop(self):
        self.cancel_motion_keepalives()
        self.header.show_stop_result("STOPPING...")
        self.ros_bridge.stop(
            self.on_stop_result
        )

    def on_stop_result(self, success, detail, _response):
        label = "E-STOP ACTIVE" if success else "STOP FAILED"
        self.header.show_stop_result(label)
        if not success:
            wx.CallLater(1200, self.header.show_stop_result, "STOP")
        if not success:
            self.show_service_error("Stop", detail)

    def set_freedrive(self, enabled):
        self.main_page.set_freedrive_pending(enabled)
        self.ros_bridge.set_freedrive(
            enabled,
            lambda success, detail, response: self.on_freedrive_result(
                enabled, success, detail, response
            ),
        )

    def on_freedrive_result(self, enabled, success, detail, _response):
        if not success:
            self.main_page.set_freedrive_failure(detail)
            self.show_service_error(
                "Enter Free Drive" if enabled else "Exit Free Drive", detail
            )
            return
        # Do not fake the final state. robot_status will set the colour and label.
        self.main_page.free_drive.SetLabel("Waiting for feedback...")

    def set_ros_control(self, enabled):
        self.ros_bridge.set_ros_control(
            enabled,
            lambda success, detail, response: self.command_result(
                "Activate ROS Control" if enabled else "Deactivate ROS Control",
                success,
                detail,
                response,
            ),
        )

    def set_speed(self, ratio):
        self.ros_bridge.set_speed(
            ratio,
            lambda success, detail, response: self.command_result(
                "Velocity Scaling", success, detail, response
            ),
        )

    def read_tcp(self):
        self.ros_bridge.get_tcp(self.on_tcp_read)

    def on_tcp_read(self, success, detail, response):
        if not success:
            self.show_service_error("Get TCP", detail)
            return
        self.tcp_page.set_pose(response.pose)

    def set_tcp(self, pose):
        if not (
            self.robot_status_received
            and self.sdk_connected
            and not self.servo_enabled
            and not self.robot_moving
        ):
            self.show_service_error("Set TCP", "Servo Off and a stationary robot are required")
            return
        self.ros_bridge.set_tcp(pose, self.on_tcp_written)

    def on_tcp_written(self, success, detail, _response):
        if not success:
            self.show_service_error("Set TCP", detail)
            return
        self.read_tcp()

    def read_safety(self):
        self.ros_bridge.get_safety(self.on_safety_read)

    def on_safety_read(self, success, detail, response):
        if success:
            self.safety_page.set_level(response.data)
        else:
            self.show_service_error("Get Safety Level", detail)

    def set_safety(self, level):
        self.ros_bridge.set_safety(level, self.on_safety_written)

    def on_safety_written(self, success, detail, _response):
        if not success:
            self.show_service_error("Set Safety Level", detail)
            return
        self.read_safety()

    def read_payload(self):
        self.ros_bridge.get_payload(self.on_payload_read)

    def on_payload_read(self, success, detail, response):
        if success:
            self.payload_page.set_payload(
                response.mass, response.center_of_gravity, response.max_payload
            )
        else:
            self.show_service_error("Get Payload", detail)

    def set_payload(self, mass, cog):
        self.ros_bridge.set_payload(mass, cog, self.on_payload_written)

    def on_payload_written(self, success, detail, _response):
        if not success:
            self.show_service_error("Set Payload", detail)
            return
        self.read_payload()

    def start_jog(self, mode, axis, direction):
        self.stop_target()
        self.stop_jog()
        direction_value = (
            Jog.Request.DIRECTION_NEGATIVE
            if direction < 0
            else Jog.Request.DIRECTION_POSITIVE
        )
        self.jog_generation += 1
        generation = self.jog_generation
        self.jog_start_deadline = time.monotonic() + 5.0
        self.active_jog = (mode, axis, direction_value)
        self.jog_request_pending = True
        self.ros_bridge.jog(mode, axis, direction_value, Jog.Request.ACTION_START,
                            lambda success, detail, response: self.on_jog_start(
                                generation, success, detail, response
                            ))

    def on_jog_start(self, generation, success, detail, _response):
        if generation != self.jog_generation:
            return
        self.jog_request_pending = False
        if not success:
            if (
                self.is_transient_motion_state(detail)
                and wx.GetMouseState().LeftIsDown()
                and time.monotonic() < self.jog_start_deadline
            ):
                wx.CallLater(250, self.retry_jog_start, generation)
                return
            self.stop_jog()
            self.show_service_error("Jog", detail)
        else:
            self.schedule_jog_keepalive()

    def retry_jog_start(self, generation):
        if generation != self.jog_generation or self.active_jog is None:
            return
        if not wx.GetMouseState().LeftIsDown():
            self.stop_jog()
            return
        mode, axis, direction = self.active_jog
        self.jog_request_pending = True
        self.ros_bridge.jog(
            mode,
            axis,
            direction,
            Jog.Request.ACTION_START,
            lambda success, detail, response: self.on_jog_start(
                generation, success, detail, response
            ),
        )

    def schedule_jog_keepalive(self):
        if self.active_jog is not None:
            self.jog_keepalive = wx.CallLater(200, self.send_jog_keepalive)

    def send_jog_keepalive(self):
        if self.active_jog is None:
            return
        if not wx.GetMouseState().LeftIsDown():
            self.stop_jog()
            return
        if self.jog_request_pending:
            self.schedule_jog_keepalive()
            return
        mode, axis, direction = self.active_jog
        generation = self.jog_generation
        self.jog_request_pending = True
        self.ros_bridge.jog(mode, axis, direction, Jog.Request.ACTION_KEEPALIVE,
                            lambda success, detail, response: self.on_jog_keepalive(
                                generation, success, detail, response
                            ))
        self.schedule_jog_keepalive()

    def on_jog_keepalive(self, generation, success, detail, _response):
        if generation != self.jog_generation:
            return
        self.jog_request_pending = False
        if not success:
            self.stop_jog()
            self.show_service_error("Jog", detail)

    def stop_jog(self):
        if self.active_jog is None:
            return
        mode, axis, direction = self.active_jog
        self.active_jog = None
        self.jog_generation += 1
        self.jog_request_pending = False
        if self.jog_keepalive is not None and self.jog_keepalive.IsRunning():
            self.jog_keepalive.Stop()
        self.ros_bridge.jog(mode, axis, direction, Jog.Request.ACTION_STOP,
                            lambda _success, _detail, _response: None)

    def start_hold_target(self, mode):
        self.stop_jog()
        self.stop_target()
        try:
            target = self.main_page.target_values(mode)
        except ValueError:
            self.show_service_error("Move Target", "All six target values must be numbers")
            return
        self.start_target_request(mode, target)

    def start_target_request(self, mode, target):
        self.target_generation += 1
        generation = self.target_generation
        self.target_start_deadline = time.monotonic() + 5.0
        self.active_target = (mode, target)
        self.target_request_pending = True
        if mode == MoveTarget.Request.MODE_ALIGN_Z:
            self.main_page.set_alignment_state("Aligning Z...", True)
        self.ros_bridge.move_target(mode, target, MoveTarget.Request.ACTION_START, True,
                                    lambda success, detail, response: self.on_target_start(
                                        generation, success, detail, response
                                    ))

    def on_target_start(self, generation, success, detail, _response):
        if generation != self.target_generation:
            return
        self.target_request_pending = False
        if not success:
            if (
                self.is_transient_motion_state(detail)
                and wx.GetMouseState().LeftIsDown()
                and time.monotonic() < self.target_start_deadline
            ):
                wx.CallLater(250, self.retry_target_start, generation)
                return
            self.stop_target()
            self.main_page.set_alignment_state("Z Align Failed")
            self.show_service_error("SDK Motion", detail)
        elif self.active_target is not None and (
            self.active_target[0] == MoveTarget.Request.MODE_ALIGN_Z
            and "already aligned" in detail.lower()
        ):
            self.stop_target()
            self.main_page.set_alignment_state("Z Already Aligned")
            wx.CallLater(
                1500, self.main_page.set_alignment_state, "Z-axis Alignment"
            )
        else:
            self.schedule_target_keepalive()

    @staticmethod
    def is_transient_motion_state(detail):
        """States seen briefly while the controller exits the previous hold mode."""
        return any(
            state in detail
            for state in (
                "RobotInLongJogMoving",
                "RobotInMoving",
                "RobotStopping",
            )
        )

    def retry_target_start(self, generation):
        if generation != self.target_generation or self.active_target is None:
            return
        if not wx.GetMouseState().LeftIsDown():
            self.stop_target()
            return
        mode, target = self.active_target
        self.target_request_pending = True
        self.ros_bridge.move_target(
            mode,
            target,
            MoveTarget.Request.ACTION_START,
            True,
            lambda success, detail, response: self.on_target_start(
                generation, success, detail, response
            ),
        )

    def schedule_target_keepalive(self):
        if getattr(self, "active_target", None) is not None:
            self.target_keepalive = wx.CallLater(200, self.send_target_keepalive)

    def send_target_keepalive(self):
        if getattr(self, "active_target", None) is None:
            return
        if not wx.GetMouseState().LeftIsDown():
            self.stop_target()
            return
        if self.target_request_pending:
            self.schedule_target_keepalive()
            return
        mode, target = self.active_target
        generation = self.target_generation
        self.target_request_pending = True
        self.ros_bridge.move_target(mode, target, MoveTarget.Request.ACTION_KEEPALIVE, True,
                                    lambda success, detail, response: self.on_target_keepalive(
                                        generation, success, detail, response
                                    ))
        self.schedule_target_keepalive()

    def on_target_keepalive(self, generation, success, detail, _response):
        if generation != self.target_generation:
            return
        self.target_request_pending = False
        if not success:
            self.stop_target()
            self.main_page.set_alignment_state("Z Align Failed")
            self.show_service_error("SDK Motion", detail)

    def stop_target(self):
        if getattr(self, "active_target", None) is None:
            return
        mode, target = self.active_target
        self.active_target = None
        self.target_generation += 1
        self.target_request_pending = False
        if self.target_keepalive is not None and self.target_keepalive.IsRunning():
            self.target_keepalive.Stop()
        self.ros_bridge.move_target(mode, target, MoveTarget.Request.ACTION_STOP, True,
                                    lambda _success, _detail, _response: None)
        if mode == MoveTarget.Request.MODE_ALIGN_Z:
            self.main_page.set_alignment_state("Z Stopped")
            wx.CallLater(
                1000, self.main_page.set_alignment_state, "Z-axis Alignment"
            )

    def start_special_target(self, mode):
        self.stop_jog()
        self.stop_target()
        target = [0.0] * 6
        self.start_target_request(mode, target)

    def on_close(self, event):
        self.Hide()
        self.stop_jog()
        self.stop_target()
        self.state_watchdog.Stop()
        self.ros_executor.shutdown(timeout_sec=1.0)
        self.ros_bridge.destroy_node()
        self.Destroy()


class ElfinGuiApp(wx.App):
    def OnInit(self):
        frame = ElfinGuiFrame()
        frame.Show()
        self.SetTopWindow(frame)
        return True


if __name__ == "__main__":
    rclpy.init()
    try:
        app = ElfinGuiApp(False)
        app.MainLoop()
    finally:
        if rclpy.ok():
            rclpy.shutdown()
