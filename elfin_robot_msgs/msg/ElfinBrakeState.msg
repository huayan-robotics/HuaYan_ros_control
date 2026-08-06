std_msgs/Header header

# Raw BrakeState values received from the controller, one value per joint.
# HR6.5.22a virtual-controller verification: HRIF_OpenBrake changes the
# selected axis from 0 to 1. This is a manual brake-open flag, not the robot FSM.
int32[6] raw_state

# Normalized physical release state for applications and the GUI. A brake is
# released during enabled operation, or when its manual-open flag is nonzero.
bool[6] released
