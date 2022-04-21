#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include "config.h"

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
      Serial.println("ERROR IN MAIN LOOP");
  }
}
void checkSerialData(){
  readSerialData();
}

bool ackReviecedMessage(){
  if(ackReviecedMessageFirstPart()){
    return true;
  }
  else{
    return false;
  }
}

bool ackReviecedMessageFirstPart(){
  //Serial.println(String(convertMessageFirstPartToInt(getRecievedSerialDataFirstPart())));
  switch(convertMessageFirstPartToInt(getRecievedSerialDataFirstPart())){
    case(Manual):
      currentState = MANUAL;
      if(ackReviecedMessageSecondPart()){
        return true;
      }
      else{
        return false;
      }
    case(Hello):
      sendMessageAck("Hello");
      return true;
    case(Stop):
      currentState = STANDBY;
      sendMessageAck("STANDBY");
      return true;
    case(Autonomous):
      currentState = AUTONOMOUS;
      sendMessageAck("AUTONOMOUS");
      return true;
    case(Error_1):
      Serial.println("Error_1");
      return false;
  }
}

bool ackReviecedMessageSecondPart(){
  switch(convertMessageSecondPartToInt(getRecievedSerialDataSecondPart())){
    case(None):
      currentDirection = NONE;
      sendMessageAck("NONE");
      return true;
    case(Forward):
      currentDirection = FORWARD;
      sendMessageAck("FORWARD");
      return true;
    case(Backward):
      currentDirection = BACKWARD;
      sendMessageAck("BACKWARD");
      return true;
    case(Left):
      currentDirection = LEFT;
      sendMessageAck("LEFT");
      return true;
    case(Right):
      currentDirection = RIGHT;
      sendMessageAck("RIGHT");
      return true;
    case(Error_2):
      Serial.println("Error_1");
      return false;
  }
}

messageFirstPart_t convertMessageFirstPartToInt(String message){
  String tempMessage = message;
  tempMessage.trim();
  
  if(tempMessage.equals("Hello")){
    return Hello;
  }
  else if(tempMessage.equals("STANDBY")){
    return Stop;
  }
  else if(tempMessage.equals("AUTONOMOUS")){
    return Autonomous;
  }
  else if(tempMessage.equals("MANUAL")){
    return Manual;
  }
  else
    return Error_1;
}

messageSecondPart_t convertMessageSecondPartToInt(String message){
  String tempMessage = message;
  tempMessage.trim();
  
  if(tempMessage.equals("NONE")){
    return None;
  }
  else if(tempMessage.equals("FORWARD")){
    return Forward;
  }
  else if(tempMessage.equals("BACKWARD")){
    return Backward;
  }
  else if(tempMessage.equals("LEFT")){
    return Left;
  }
  else if(tempMessage.equals("RIGHT")){
    return Right;
  }
  else
    return Error_2;
}

void doManualControlTick(){
  //moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentSpeedLeftMotor, currentAngle),calculateRightMotorSpeed(currentSpeedRightMotor, currentAngle));
  move(currentDirection, MOTOR_SPEED_MANUAL);
}
