void autoMove(){
  getSerialData();
  int leftOrRight = random(3, 5);
  
  if(ultraSonicSensor.distanceCm() < 10){
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
  if(lineFollowerSensor.readSensors() == 0.000000){
      Serial.println("IR sensor triggered");
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      _delay(0.5);

      move(BACKWARD, 40 / 100.0 * 255);
      _delay(1);
      move(BACKWARD, 0);
  
      move(leftOrRight, 30 / 100.0 * 255);
      _delay(random(1, 3));
      move(leftOrRight, 0);

  }

  move(FORWARD, 40 / 100.0 * 255);

  _loop();
}
