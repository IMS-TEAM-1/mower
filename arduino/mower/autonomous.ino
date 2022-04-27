/*
 * The following "doAutonomousTick()" is the "main-code" for the mower when operating autonomously.
 * It simply moves forward at all times.
 * However, if either the ultrasonic sensor or IR-sensors gets triggered, then the mower should act accordingly.
 * 
 * This function is called continously when in autonomous-state.
 */

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
  //_delay(1);
}

//This function simply does a simple reversing manuever
void doReverseProcedure(){
  activateAutonomousLEDs();
  move(BACKWARD, MOTOR_SPEED_AUTONOMOUS_BACKWARD * PERCENTAGE_TO_PWM_FACTOR);
  _delay(1);
  stopMotors();

  activateAutonomousLEDs();
  
  move(randomLeftOrRight(), MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT * PERCENTAGE_TO_PWM_FACTOR);
  _delay(random(1, 3));
  stopMotors();
  activateAutonomousLEDs();
}


/*
 * This function is called when the Arduino should wait for the Pi to take the picture through the camera.
 * The mower stand still and waits for an acknowledgement.
 * After a set time-out value, the mower continoues anyway.
 */
void waitForImageCaptured(){
  stopMotors();
  bool doLoop = true;
  long timeToCapture = millis() + CAMERA_CAPTURE_TIME;
  
  while(doLoop){
    stopMotors();
    doSerialTick();
    if(recievedCaptureAck()){
      doLoop = false;
    }
    else if(millis() > timeToCapture) {
      Serial.println("ERROR IN CAMERA CAPTURE");
      doLoop = false;
    }
  }
}
