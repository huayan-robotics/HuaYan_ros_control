std_msgs/Header header
# Array index is the end-tool channel number. -1 means that channel read failed.
int32[] digital_inputs
int32[] digital_outputs
int32[] buttons
bool buttons_enabled
# NaN means that channel read failed.
float64[] analog_inputs
