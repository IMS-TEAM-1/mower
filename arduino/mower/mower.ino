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
MeRGBLed rgbled_0(0, 6);

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
  readSerialData();
  
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
      activateManualLEDs();
      doManualControlTick();
      break;
  }
}

bool ackReviecedMessageFirstPart(){
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
      return false;
  }
}

messageFirstPart_t convertMessageFirstPartToInt(String message){
  message.trim();
  
  if(message.equals("Hello")){
    return Hello;
  }
  else if(message.equals("STANDBY")){
    return Stop;
  }
  else if(message.equals("AUTONOMOUS")){
    return Autonomous;
  }
  else if(message.equals("MANUAL")){
    return Manual;
  }
  else
    return Error_1;
}

messageSecondPart_t convertMessageSecondPartToInt(String message){
  message.trim();
  
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
    return Error_2;
}

void doManualControlTick(){
  //moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentSpeedLeftMotor, currentAngle),calculateRightMotorSpeed(currentSpeedRightMotor, currentAngle));
  move(currentDirection, MOTOR_SPEED_MANUAL * PERCENTAGE_TO_PWM_FACTOR);
}
