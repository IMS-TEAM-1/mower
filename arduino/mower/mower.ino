#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include "config.h"

float timeAtLastUpdate = 0;

MeUltrasonicSensor ultraSonicSensor(ULTRASONIC_SENSOR_PORT);
MeLineFollower lineFollowerSensor(LINE_FOLLOWER_SENSOR_PORT);

MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

typedef enum {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT } direction_t;

void setupSensors(){
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
}

//Initiliazes sensors, serial, ports etc.
void setup() {
  setupSerial();
  setupSensors();
  setupMotors();

  //Is this really needed?
  //randomSeed((unsigned long)(lightsensor_12.read() * 123456));
}

//Main loop, seen as the main program of the MCU
void loop() {
  autoMove();
}
