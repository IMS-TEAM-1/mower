#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include "config.h"

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

typedef enum {
  Hello,
  Standby,
  Autonomous,
  Manual } message1_t;

typedef enum {
  None,
  Forward,
  Backward,
  Left,
  Right } message2_t;

float timeAtLastUpdate = 0;
const char MAX_MESSAGE_LENGTH = 12;

mowerState_t currentState = STANDBY;
direction_t currentDirection = NONE;

MeUltrasonicSensor ultraSonicSensor(ULTRASONIC_SENSOR_PORT);
MeLineFollower lineFollowerSensor(LINE_FOLLOWER_SENSOR_PORT);

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

//Initiliazes sensors, serial, ports etc.
void setup() {
  setupSerial();
  setupEncoderInterrupts();
  setupMotors();
  currentState = STANDBY;
  //Is this really needed?
  //randomSeed((unsigned long)(lightsensor_12.read() * 123456));
}

//Main loop, seen as the main program of the MCU
void loop() {
  checkSerialData();
  
  switch(currentState){
    case(STANDBY):
      resetMotorValues();
      break;
    case(AUTONOMOUS):
      //In autonomous mode
      doAutonomousTick();
      break;
    case(MANUAL):
      //In manual mode
      doManualControlTick();
      break;
    default:
      while(true) Serial.println("ERROR IN MAIN LOOP");
  }
}
void checkSerialData(){
  readSerialData();
  
  switch(convertMessage1ToInt(getRecievedSerialData1())){
    case(Hello):
      sendMessageAck("Hello");
      break;
    case(Standby):
      currentState = STANDBY;
      sendMessageAck("STANDBY");
      break;
    case(Autonomous):
      currentState = AUTONOMOUS;
      sendMessageAck("AUTONOMOUS");
      break;
    case(Manual):
      currentState = MANUAL;

      switch(convertMessage2ToInt(getRecievedSerialData2())){
        case(None):
          currentDirection = NONE;
          sendMessageAck("NONE");
          break;
        case(Forward):
          currentDirection = FORWARD;
          sendMessageAck("FORWARD");
          break;
        case(Backward):
          currentDirection = BACKWARD;
          sendMessageAck("BACKWARD");
          break;
        case(Left):
          currentDirection = LEFT;
          sendMessageAck("LEFT");
          break;
        case(Right):
          currentDirection = RIGHT;
          sendMessageAck("RIGHT");
          break;
      }
      
      break;
  }
}

message1_t convertMessage1ToInt(String message){
  if(message.equals("Hello")){
    return Hello;
  }
  else if(message.equals("STANDBY")){
    return Standby;
  }
  else if(message.equals("AUTONOMOUS")){
    return Autonomous;
  }
  else if(message.equals("MANUAL")){
    return Manual;
  }
  else
    while(true) Serial.println("ERROR IN CONVERSION OF MESSAGE IN mower.ino");
}

message2_t convertMessage2ToInt(String message){
  if(message.equals("NONE")){
    return None;
  }
  else if(message.equals("FORWARD")){
    return Forward;
  }
  else if(message.equals("BACKWARD")){
    return Backward;
  }
  else if(message.equals("LEFT")){
    return Left;
  }
  else if(message.equals("RIGHT")){
    return Right;
  }
  else
    while(true) Serial.println("ERROR IN CONVERSION OF MESSAGE IN mower.ino");
}

void doManualControlTick(){
  //moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentSpeedLeftMotor, currentAngle),calculateRightMotorSpeed(currentSpeedRightMotor, currentAngle));
  move(currentDirection, MOTOR_SPEED_MANUAL);
}
