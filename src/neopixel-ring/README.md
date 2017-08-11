# Ambient light for the neopixel ring with 24 rgb leds

### Compile
`make`

### Compile and upload:
`make upload`

### Start pyscript that converts serial ros messages to http:
`rosrun rosserial_python serial_node.py /dev/ttyACM0`

### Set depth:
`rostopic pub --once depth std_msgs/Float32 0`
