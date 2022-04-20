void doAutonomousTick(){
  move(FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD);
  
  if(getUltraSonicSensorTriggered()){
      stopMotors();

      sendSerialUltraSonicTriggered();
      waitForImageCaptured();

      doReverseProcedure();
  }
  if(getLineFollowerTriggered()){
      stopMotors();
      
      _delay(0.5);

      doReverseProcedure();
  }
  //What is this?
  _loop();
}

void doReverseProcedure(){
  move(BACKWARD, MOTOR_SPEED_AUTONOMOUS_BACKWARD * PERCENTAGE_TO_PWM_FACTOR);
  _delay(1);
  stopMotors();

  move(randomLeftOrRight(), MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT * PERCENTAGE_TO_PWM_FACTOR);
  _delay(random(1, 3));
  stopMotors();
}

void waitForImageCaptured(){
  int timeToCapture = millis() + CAMERA_CAPTURE_TIME;
  while(true){
    if(recievedCaptureAck()){
      break;
    }
    else if(millis() < (unsigned)timeToCapture)
      Serial.println("ERROR IN CAMERA CAPTURE");
      break;
  }
}
