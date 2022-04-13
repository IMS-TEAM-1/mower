//Calculated frequency in ms, deciding update frequency during serial communication.
#define SERIAL_UPDATE_FREQUENCY_MS 50

//Sensor ports
#define ULTRASONIC_SENSOR_PORT 10
#define LINE_FOLLOWER_SENSOR_PORT 9

//Distance from object it should not detect and avoid
#define ULTRA_SONIC_SENSOR_DISTANCE_CM 10

//Motor constant, converts 0-100% to a value of 0-255 for the PWM control
#define PERCENTAGE_TO_PWM_FACTOR 2.5
