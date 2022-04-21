void doAutonomousTick(){
  if(getUltraSonicSensorTriggered()){
      stopMotors();

      sendSerialUltraSonicTriggered();
      waitForImageCaptured();

      doReverseProcedure();
  }
  if(getLineFollowerTriggered()){
      stopMotors();

      doReverseProcedure();
  }
  move(FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  //What is this?
  //Serial.println("TEST");
  //_delay(1);
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
  stopMotors();
  int timeToCapture = millis() + CAMERA_CAPTURE_TIME;
  while(true){
    readSerialData();
    if(recievedCaptureAck()){
      break;
    }
    else if(millis() > (unsigned)timeToCapture)
      Serial.println("ERROR IN CAMERA CAPTURE");
      return;
  }
}
