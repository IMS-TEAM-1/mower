/*
 * This file contains constants, enums and similar used throughout the project.
 * It is here most parameters are changed when changed behaviour of the mower is wanted.
 */

/*
 * Typedef enums
 */

//What state the mower is in right now and what direction the mower should go for when moving
typedef enum {
  STANDBY,
  AUTONOMOUS,
  MANUAL,
  DIAGNOSTIC} mowerState_t;
typedef enum {
  NONE,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT } direction_t;

//Enums of what messages the mower can recieve and respond to
//typedef enum {
//  Hello,
//  Stop,
//  Autonomous,
//  Manual,
//  Diagnostic,
//  Error_1} messageFirstPart_t;
//
//typedef enum {
//  None,
//  Forward,
//  Backward,
//  Left,
//  Right,
//  Error_2,} messageSecondPart_t;

typedef enum {
  Hello,
  Standby,
  Autonomous,
  ManualStop,
  ManualForward,
  ManualBackward,
  ManualLeft,
  ManualRight,
  Diagnostic,
  Error } messageRecieved_t;

//Enum used in diagnostics when trouble shooting the mower
typedef enum {
  Diag_Main,
  Diag_Encoder,
  Diag_Gyro,
  Diag_Joystick,
  Diag_Localization,
  Diag_Motorcontrol,
  Diag_Sensors,
  Diag_SerialCom,
  Diag_Autonomous,
  Diag_Done} diagntosicModule_t;


/*
 * Defines
 */

//Serial defines
#define SERIAL_UPDATE_FREQUENCY_MS 50
#define MAX_ALLOWED_MISSED_SERIAL_TICKS 10


//Sensor ports
#define ULTRASONIC_SENSOR_PORT 10
#define LINE_FOLLOWER_SENSOR_PORT 9


//Distance from object it should not detect and avoid
#define ULTRA_SONIC_SENSOR_DISTANCE_CM 10


//Motor constants
#define LOCALIZATION_CIRCLE_ROTATION_OFFSET 90.0

#define ROTATING_LEFT_MOMENTUM_OFFSET 13
#define ROTATING_RIGHT_MOMENTUM_OFFSET 13

#define AUTONOMOUS_LOCATION_TICK_TIME_MS 1000

#define MILLIMETER_DISTANCE_WHEN_FREE_ROLLING_AFTER_FULL_SPEED 45

#define PERCENTAGE_TO_PWM_FACTOR 2.55
#define MOTOR_DEVIATION_FACTOR 0.95
#define DEGREES_TO_RADIAN_FACTOR M_PI/180

#define MILLIMETER_PER_ENCOER_PULSE 0.353
#define ENCODER_PULSE_PER_MILLIMETER 2.832

#define MAX_MOTOR_SPEED 255
#define HALF_MOTOR_SPEED 255*0.5
#define MOTOR_SPEED_MANUAL 100
#define MOTOR_SPEED_AUTONOMOUS_FORWARD 60
#define MOTOR_SPEED_AUTONOMOUS_BACKWARD 60
#define MOTOR_SPEED_AUTONOMOUS_LEFT 60
#define MOTOR_SPEED_AUTONOMOUS_RIGHT 60
#define MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT 60

#define ENCODER_LIBRARY_PWM_OFFSET_VALUE 2


//Camera capture time, in ms
#define CAMERA_CAPTURE_TIME 10000


//Diagnostics constans
#define MAX_AMOUNT_OF_FAULT_CODES 20
#define TIME_TO_TEST_AUTONOMOUS_STATE_MS 15000
#define ULTRA_SONIC_SENSOR_MAX_VALUE 400
#define TIME_TO_TEST_ULTRA_SONIC_SENSOR_MS 10000
