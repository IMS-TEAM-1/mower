//Typedef enums
//What state the mower is in right now and what direction the mower should go for when moving
typedef enum {
  STANDBY,
  AUTONOMOUS,
  MANUAL } mowerState_t;
typedef enum {
  NONE,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT } direction_t;

//Enums of what messages the mower can recieve and respond to
typedef enum {
  Hello,
  Standby,
  Autonomous,
  Manual } messageFirstPart_t;

typedef enum {
  None,
  Forward,
  Backward,
  Left,
  Right } messageSecondPart_t;

//Calculated frequency in ms, deciding update frequency during serial communication.
#define SERIAL_UPDATE_FREQUENCY_MS 50

//Sensor ports
#define ULTRASONIC_SENSOR_PORT 10
#define LINE_FOLLOWER_SENSOR_PORT 9

//Distance from object it should not detect and avoid
#define ULTRA_SONIC_SENSOR_DISTANCE_CM 10

//Motor constant, converts 0-100% to a value of 0-255 for the PWM control
#define PERCENTAGE_TO_PWM_FACTOR 2.5

//Motor speed constant
#define MOTOR_SPEED_MANUAL 100

#define MOTOR_SPEED_AUTONOMOUS_FORWARD 100
#define MOTOR_SPEED_AUTONOMOUS_BACKWARD 40
#define MOTOR_SPEED_AUTONOMOUS_LEFT 100
#define MOTOR_SPEED_AUTONOMOUS_RIGHT 100
#define MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT 100

//Camera capture time, in ms
#define CAMERA_CAPTURE_TIME 10000
