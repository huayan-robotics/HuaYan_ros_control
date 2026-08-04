#!/usr/bin/env bash
# Collect read-only diagnostics for the Elfin controller/MoveIt data path.
set -u

ROBOT_IP="${1:-10.20.215.133}"
OUTPUT_FILE="${2:-elfin_diagnostics_$(date +%Y%m%d_%H%M%S).txt}"

section()
{
  printf '\n========== %s ==========\n' "$1" | tee -a "$OUTPUT_FILE"
}

run()
{
  printf '\n$ %s\n' "$*" | tee -a "$OUTPUT_FILE"
  "$@" 2>&1 | tee -a "$OUTPUT_FILE" || true
}

: > "$OUTPUT_FILE"
section "SYSTEM"
run date --iso-8601=seconds
run uname -a
run bash -c 'printf "ROS_DISTRO=%s\nROS_DOMAIN_ID=%s\nRMW_IMPLEMENTATION=%s\n" "${ROS_DISTRO:-}" "${ROS_DOMAIN_ID:-}" "${RMW_IMPLEMENTATION:-}"'

section "PACKAGE PREFIXES"
run ros2 pkg prefix elfin_robot_bringup
run ros2 pkg prefix elfin_controller_driver
run ros2 pkg prefix elfin_robot_msgs
run ros2 pkg prefix joint_state_broadcaster
run ros2 pkg prefix joint_trajectory_controller

section "NETWORK"
run timeout 3 nc -vz "$ROBOT_IP" 8892
run timeout 3 nc -vz "$ROBOT_IP" 8893
run timeout 3 nc -vz "$ROBOT_IP" 10003
run timeout 3 nc -vz "$ROBOT_IP" 10004

section "ROS GRAPH"
run ros2 node list
run ros2 topic list -t
run ros2 service list -t
run ros2 action list -t

section "ROS2 CONTROL"
run ros2 control list_hardware_components -v
run ros2 control list_controllers -v
run ros2 control list_hardware_interfaces

section "TOPIC ENDPOINTS"
run ros2 topic info /joint_states -v
run ros2 topic info /elfin_sdk/joint_states -v
run ros2 topic info /elfin_sdk/realtime_state -v
run ros2 topic info /elfin_sdk/robot_status -v

section "TOPIC SAMPLES"
run timeout 6 ros2 topic echo --once /joint_states
run timeout 6 ros2 topic echo --once /elfin_sdk/joint_states
run timeout 6 ros2 topic echo --once /elfin_sdk/realtime_state
run timeout 6 ros2 topic echo --once /elfin_sdk/robot_status

section "TOPIC RATES"
run timeout 8 ros2 topic hz /joint_states --window 10
run timeout 8 ros2 topic hz /elfin_sdk/realtime_state --window 10

section "MOVEIT AND TRAJECTORY ACTIONS"
run ros2 action info /move_action
run ros2 action info /execute_trajectory
run ros2 action info /elfin_arm_controller/follow_joint_trajectory
run ros2 param get /move_group use_sim_time
run ros2 param get /robot_state_publisher use_sim_time
run ros2 param get /controller_manager update_rate

section "TF"
run timeout 5 ros2 run tf2_ros tf2_echo world elfin_end_link

section "LATEST ROS LOG ERRORS"
LATEST_LOG="$(find "${ROS_LOG_DIR:-$HOME/.ros/log}" -mindepth 1 -maxdepth 1 -type d -printf '%T@ %p\n' 2>/dev/null | sort -nr | head -1 | cut -d' ' -f2-)"
if [[ -n "$LATEST_LOG" && -d "$LATEST_LOG" ]]; then
  run bash -c 'if command -v rg >/dev/null; then rg -n "ERROR|FATAL|WARN|failed|Failed|invalid|timeout|8892|8893" "$1"; else grep -RniE "ERROR|FATAL|WARN|failed|invalid|timeout|8892|8893" "$1"; fi || true' _ "$LATEST_LOG"
else
  printf 'No ROS log directory found.\n' | tee -a "$OUTPUT_FILE"
fi

section "SUMMARY HINTS"
printf '%s\n' \
  '1. Hardware must be active.' \
  '2. joint_state_broadcaster must be active.' \
  '3. /elfin_sdk/realtime_state and /joint_states must both have messages.' \
  '4. joint_position_actual must change when the physical arm moves.' \
  '5. MoveIt must expose /move_action; trajectory execution also needs the FollowJointTrajectory server.' \
  | tee -a "$OUTPUT_FILE"

printf '\nDiagnostics saved to: %s\n' "$OUTPUT_FILE"
