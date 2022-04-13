void doAutonomousTick(){
  int leftOrRight = random(3, 5);
  
  if(getUltraSonicSensorTriggered()){
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      _delay(0.5);

      move(BACKWARD, 40 / 100.0 * 255);
      _delay(1);
      move(BACKWARD, 0);

      move(randomLeftOrRight(), 30 / 100.0 * 255);
      _delay(random(1, 3));
      move(randomLeftOrRight(), 0);

  }
  if(getLineFollowerTriggered()){
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      _delay(0.5);

      move(BACKWARD, 40 / 100.0 * 255);
      _delay(1);
      move(BACKWARD, 0);
  
      move(leftOrRight, 30 / 100.0 * 255);
      _delay(random(1, 3));
      move(randomLeftOrRight(), 0);

  }

  move(FORWARD, 40 / 100.0 * 255);

  _loop();
}
