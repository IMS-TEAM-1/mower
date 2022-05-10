/*
 * The following "doAutonomousTick()" is the "main-code" for the mower when operating autonomously.
 * It simply moves forward at all times.
 * However, if either the ultrasonic sensor or IR-sensors gets triggered, then the mower should act accordingly.
 * 
 * This function is called continously when in autonomous-state.
 */
void doAutonomousTick(){
  //If any of the sensors are triggered, stop, and do the appropiate procedure
  if(getUltraSonicSensorTriggered()){
    doAutonomousUltraSonicProcedure();
  }
  if(getLineFollowerTriggered()){
    doAutonomousLineFollowerProcedure();
  }

  //If we should send our location to the Pi, do that
  doLocalizationTick();  

  //Else, always move forward
  activateAutonomousForwardLEDs();
  move(FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
}

//If line follower triggered, do this
void doAutonomousLineFollowerProcedure(){
  stopMotors();

  _delay(1);

  calculateAndUpdateXAndYCoordinates();

  doReverseProcedure();
}

//If ultra sonic sensor triggered, do this
void doAutonomousUltraSonicProcedure(){
  stopMotors();

  _delay(1);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialUltraSonicTriggered();
  
  waitForImageCaptured();

  doReverseProcedure();
}

//This function simply does a simple reversing manuever
void doReverseProcedure(){
  activateAutonomousLEDs();

  driveTime(1500, BACKWARD, MOTOR_SPEED_AUTONOMOUS_BACKWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  stopMotors();

  _delay(1);

  activateAutonomousLEDs();
  
  rotateByDegrees(random(110, 250), randomLeftOrRight(), MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT * PERCENTAGE_TO_PWM_FACTOR); 
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
    doSerialTick(false);
    if(recievedCaptureAck()){
      doLoop = false;
    }
    else if(millis() > timeToCapture) {
      Serial.println(getRecievedSerialDataFirstPart());
      Serial.println("ERROR IN CAMERA CAPTURE");
      doLoop = false;
    }
  }
}




int doAutonomousLineFollowerProcedureTest(){
  int errorCounter = 0;

  if(!resetStateLEDsTest()){errorCounter ++;}
  
  stopMotors();

  _delay(1);

  errorCounter += doReverseProcedureTest();

  return errorCounter;
}

int doReverseProcedureTest(){
  int errorCounter = 0;

  if(!resetStateLEDsTest()){errorCounter ++;}
  
  stopMotors();

  _delay(1);

  driveTime(1500, BACKWARD, MOTOR_SPEED_AUTONOMOUS_BACKWARD * PERCENTAGE_TO_PWM_FACTOR);

  if(!resetStateLEDsTest()){errorCounter ++;}

  
  _delay(random(1, 3));
  rotateByDegrees(random(110, 250), randomLeftOrRight(), MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT * PERCENTAGE_TO_PWM_FACTOR); 
  stopMotors();
  _delay(random(1, 3));
  if(!resetStateLEDsTest()){errorCounter ++;}
  
  return errorCounter;
}

bool waitForImageCapturedTest(){
  bool goneThroughLoop = false;
  bool resetLedFailed = false;

  bool doLoop = true;
  long timeToCapture = millis() + CAMERA_CAPTURE_TIME;
  
  while(doLoop){
    inDiagModuleLED(1);
    stopMotors();
    if(!resetStateLEDsTest()){resetLedFailed = true;}
    if(millis() > timeToCapture) {
      doLoop = false;
      goneThroughLoop = true;
    }
  }

  if(goneThroughLoop && resetLedFailed){
    return true;
  }
  else{
    return false;
  }
}

int doAutonomousUltraSonicProcedureTest(){
  int errorCounter = 0;

  if(!resetStateLEDsTest()){errorCounter++;}
  stopMotors();
  _delay(1);
  
  if(!waitForImageCapturedTest()){
    errorCounter += 1;
  }

  errorCounter += doReverseProcedureTest();

  return errorCounter;
}

int doAutonomousTickTest(){
  int errorCounter = 0;

  if(getUltraSonicSensorTriggered()){
    errorCounter += doAutonomousUltraSonicProcedureTest();
  }
  if(getLineFollowerTriggered()){
    errorCounter += doAutonomousLineFollowerProcedureTest();
  } 

  errorCounter += activateAutonomousForwardLEDsTest();
  
  move(FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  inDiagModuleLED(1);
}

bool autonomousDiagnosticTestSuccess(){
  int timeToTestAutonomousState = 15000;
  long timeToStopTest = millis() + timeToTestAutonomousState;
  int amountOfErrors = 0;

  while(millis() < timeToStopTest){
    Serial.println("TEST");
    amountOfErrors += doAutonomousTickTest();
  }

  if(amountOfErrors > 0){
    return false;
  }
  else{
    return true;
  }
}
