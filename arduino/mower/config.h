/*
 * This file contains constants, enums and similar used throughout the project.
 * It is here most parameters are changed when changed behaviour of the mower is wanted.
 */

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
  Stop,
  Autonomous,
  Manual,
  Error_1} messageFirstPart_t;

typedef enum {
  None,
  Forward,
  Backward,
  Left,
  Right,
  Error_2,} messageSecondPart_t;

//Serial defines
#define SERIAL_UPDATE_FREQUENCY_MS 50
#define MAX_ALLOWED_MISSED_SERIAL_TICKS 10

//Sensor ports
#define ULTRASONIC_SENSOR_PORT 10
#define LINE_FOLLOWER_SENSOR_PORT 9

//Distance from object it should not detect and avoid
#define ULTRA_SONIC_SENSOR_DISTANCE_CM 10

//Motor constant, converts 0-100% to a value of 0-255 for the PWM control
#define LOCALIZATION_CIRCLE_ROTATION_OFFSET 90

#define AUTONOMOUS_LOCATION_TICK_TIME 1

#define MILLIMETER_DISTANCE_WHEN_FREE_ROLLING_AFTER_FULL_SPEED 45

#define PERCENTAGE_TO_PWM_FACTOR 2.5

#define MILLIMETER_PER_ENCOER_PULSE 0.353
#define ENCODER_PULSE_PER_MILLIMETER 2.832

#define MOTOR_DEVIATION_FACTOR 0.90
#define MAX_MOTOR_SPEED 255

#define MOTOR_SPEED_MANUAL 100

#define MOTOR_SPEED_AUTONOMOUS_FORWARD 50
#define MOTOR_SPEED_AUTONOMOUS_BACKWARD 50
#define MOTOR_SPEED_AUTONOMOUS_LEFT 50
#define MOTOR_SPEED_AUTONOMOUS_RIGHT 50
#define MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT 50

//Camera capture time, in ms
#define CAMERA_CAPTURE_TIME 10000
