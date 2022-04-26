#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include "config.h"

mowerState_t currentState = STANDBY;
direction_t currentDirection = NONE;

MeUltrasonicSensor ultraSonicSensor(ULTRASONIC_SENSOR_PORT);
MeLineFollower lineFollowerSensor(LINE_FOLLOWER_SENSOR_PORT);
MeRGBLed rgbled_0(0, 12);

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

//Initiliazes sensors, serial, ports etc.
void setup() {
  setupSerial();
  setupEncoderInterrupts();
  setupMotors();
  currentState = STANDBY;
  rgbled_0.setpin(44);
  //Is this really needed?
  //randomSeed((unsigned long)(lightsensor_12.read() * 123456));
}

//Main loop, seen as the main program of the MCU
void loop() {
  doSerialTick();
  
  switch(currentState){
    case(STANDBY):
      activateStandbyLEDs();
      stopMotors();
      break;
    case(AUTONOMOUS):
      activateAutonomousLEDs();
      doAutonomousTick();
      break;
    case(MANUAL):
      deactivateLEDs();
      doManualControlTick();
      break;
  }
}
