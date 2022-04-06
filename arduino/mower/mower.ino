#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeAuriga.h>
#include <Math.h>
#include "config.h"

MeLineFollower linefollower_9(LINE_FOLLOWER_SENSOR_PORT);
MeUltrasonicSensor ultrasonic_10(ULTRASONIC_SENSOR_PORT);
MeEncoderOnBoard Encoder_1(SLOT1);
MeEncoderOnBoard Encoder_2(SLOT2);

typedef enum {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT } direction_t;

float timeAtLastUpdate = 0;


void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0){
    Encoder_1.pulsePosMinus();
  }else{
    Encoder_1.pulsePosPlus();
  }
}
void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.getPortB()) == 0){
    Encoder_2.pulsePosMinus();
  }else{
    Encoder_2.pulsePosPlus();
  }
}
void move(direction_t direction, int speed)
{
  int leftSpeed = 0;
  int rightSpeed = 0;
  if(direction == FORWARD){
    leftSpeed = -speed;
    rightSpeed = speed;
  }else if(direction == BACKWARD){
    leftSpeed = speed;
    rightSpeed = -speed;
  }else if(direction == LEFT){
    leftSpeed = -speed;
    rightSpeed = -speed;
  }else if(direction == RIGHT){
    leftSpeed = speed;
    rightSpeed = speed;
  }
  Encoder_1.setTarPWM(leftSpeed);
  Encoder_2.setTarPWM(rightSpeed);
}

void _delay(float seconds) {
  if(seconds < 0.0){
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void setup() {
  Serial.begin(9600); //Starta Serial
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
  attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
  randomSeed((unsigned long)(lightsensor_12.read() * 123456));

}

void getSerialData(){
  if (millis() - timeAtLastUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    timeAtLastUpdate = millis();
    Serial.println("update");
  }
}

direction_t randomLeftOrRight(){
  int leftOrRight = random(3, 5);

  if (leftOrRight = 3){
    return LEFT;
  }else{
    return RIGHT;
  }
}

void autoMove(){
  int leftOrRight = random(3, 5);
  
  if(ultrasonic_10.distanceCm() < 10){
      Serial.println("Ultrasonic sensor triggered");
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      _delay(0.5);

      move(BACKWARD, 40 / 100.0 * 255);
      _delay(1);
      move(BACKWARD, 0);

      move(randomLeftOrRight(), 30 / 100.0 * 255);
      _delay(random(1, 3));
      move(leftOrRight, 0);

  }
  if(linefollower_9.readSensors() == 0.000000){
      Serial.println("IR sensor triggered");
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      _delay(0.5);

      move(2, 40 / 100.0 * 255);
      _delay(1);
      move(2, 0);
  
      move(leftOrRight, 30 / 100.0 * 255);
      _delay(random(1, 3));
      move(leftOrRight, 0);

  }

  move(1, 40 / 100.0 * 255);

  _loop();
}

void _loop() {
  Encoder_1.loop();
  Encoder_2.loop();
}

void loop() {
  getSerialData();
  autoMove();
}
