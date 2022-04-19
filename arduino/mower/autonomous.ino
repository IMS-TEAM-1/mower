void doAutonomousTick(){
  int leftOrRight = random(3, 5);
  
  if(getUltraSonicSensorTriggered()){
      move(NONE, 0);

      sendSerialUltraSonicTriggered();

      int timeToCapture = millis() + CAMERA_CAPTURE_TIME;
      while(true){
        if(recievedCaptureAck()){
          break;
        }
        else if(millis() < timeToCapture)
          Serial.println("ERROR IN CAMERA CAPTURE");
          break;
      }

      move(BACKWARD, MOTOR_SPEED_AUTONOMOUS_BACKWARD * PERCENTAGE_TO_PWM_FACTOR);
      _delay(1);
      move(NONE, 0);

      move(randomLeftOrRight(), MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT * PERCENTAGE_TO_PWM_FACTOR);
      _delay(random(1, 3));
      move(NONE, 0);

  }
  if(getLineFollowerTriggered()){
      Encoder_1.setTarPWM(0);
      Encoder_2.setTarPWM(0);
      _delay(0.5);

      move(BACKWARD, 40 * PERCENTAGE_TO_PWM_FACTOR);
      _delay(1);
      move(BACKWARD, 0);
  
      move(leftOrRight, 30 * PERCENTAGE_TO_PWM_FACTOR);
      _delay(random(1, 3));
      move(randomLeftOrRight(), 0);

  }

  move(FORWARD, 40 * PERCENTAGE_TO_PWM_FACTOR);

  _loop();
}
