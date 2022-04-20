#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include "config.h"

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
  //Is this really needed?
  //randomSeed((unsigned long)(lightsensor_12.read() * 123456));
}

//Main loop, seen as the main program of the MCU
void loop() {
  checkSerialData();
  
  switch(currentState){
    case(STANDBY):
      resetMotorValues();
      stopMotors();
      _delay(1);
      break;
    case(AUTONOMOUS):
      doAutonomousTick();
      break;
    case(MANUAL):
      doManualControlTick();
      break;
    //Remove when bug-free
    default:
      Serial.println("ERROR IN MAIN LOOP");
      break;
  }
}
void checkSerialData(){
  readSerialData();

  if(messageAcked()){
    return;
  }
  else{
    Serial.print("fail");
  }
}

bool messageAcked(){
  if(ackMessage(getRecievedSerialDataFirstPart())){
    return true;
  }
  else{
    return false;
  }
}

void doManualControlTick(){
  //moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentSpeedLeftMotor, currentAngle),calculateRightMotorSpeed(currentSpeedRightMotor, currentAngle));
  move(currentDirection, MOTOR_SPEED_MANUAL);
}
