#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include "config.h"

/*
 * Following are variables used as global variables which determine what the mower shoud do.
 * "currentState" and "currentDirection" is the primary source for the mower for what it should do when acting or moving.
 * These are defined by the typedef enum "mowerState_t" and "direction_t" which have various parameters.
 * 
 * Example: currentState = STANDBY; - this makes sure that the mower changes to standby-mode in the global loop-function.
 * 
 * Example: currentDirection = NONE; - this makes sure that the mower does not move in any direction, even if it is told to do so with a ceratain speed.
 * 
 * 
 * Beneath are decalarations on where the sensors, the motors and the RBG-LEDs are connected in the Arduino.
 * How to use can be found in repective .ino file.
 */
mowerState_t currentState = STANDBY;
direction_t currentDirection = NONE;

MeUltrasonicSensor ultraSonicSensor(ULTRASONIC_SENSOR_PORT);
MeLineFollower lineFollowerSensor(LINE_FOLLOWER_SENSOR_PORT);
MeRGBLed rgbled_0(0, 12);
MeGyro gyro(9, 0x69);

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

//Initiliazes sensors, serial, ports etc.
void setup() {
  setupSerial();
  setupEncoderInterrupts();
  setupMotors();
  currentState = STANDBY;
  rgbled_0.setpin(44);
  gyro.begin();
  //Is this really needed?
  //randomSeed((unsigned long)(lightsensor_12.read() * 123456));
}

/*
 * The following "loop()" is the main program of the Arduino.
 * 
 * It initializes by performing a serial tick, meaning that it will firstly check whether the Arduino should update based on timed
 * update-times but secondly read the data and store it for further use, seen in serial.ino
 * 
 * The loop is designed to be structured in various self-explanatory states such as: standby, autonomous and manual.
 * The standby-mode is simply a state where the mower is stationary and simply awaits orders.
 * Autonomous is the self-driving function and the code for that can be found in autonomous.ino.
 * Manual is the state where you MANUALLY control the mower via bluetooth.
 */
void loop() {
  doSerialTick(true);
  
  switch(currentState){
    case(STANDBY):
      activateStandbyLEDs();
      stopMotors();
      break;
    case(AUTONOMOUS):
      activateAutonomousForwardLEDs();
      doAutonomousTick();
      break;
    case(MANUAL):
      deactivateLEDs();
      //gyroPrintValues();
      //printEncoderPulseValues();
      doManualControlTick();
      break;
  }

  clearMessages();
}
