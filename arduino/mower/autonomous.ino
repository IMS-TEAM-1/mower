/*
 * The following "doAutonomousTick()" is the "main-code" for the mower when operating autonomously.
 * It simply moves forward at all times.
 * However, if either the ultrasonic sensor or IR-sensors gets triggered, then the mower should act accordingly.
 * 
 * This function is called continously when in autonomous-state.
 * 
 */
bool imageCaptured = false;
 
void doAutonomousTick(){
  //If any of the sensors are triggered, stop, and do the appropiate procedure
  if(getUltraSonicSensorTriggered()){
    doAutonomousUltraSonicProcedure();
  }
  if(getLineFollowerTriggered()){
    doAutonomousLineFollowerProcedure();
  }

  doLocalizationTick();  //If we should send our location to the Pi, do that

  activateAutonomousForwardLEDs();
  move(FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR); //Always move forward
}

//If line follower triggered, do this
void doAutonomousLineFollowerProcedure(){
  stopMotorsMS(1000); //Make sure that the mower has stopped

  calculateAndUpdateXAndYCoordinates(); //Send the current position to the Pi

  doReverseProcedure(); //Back up, rotate, and continue moving autonomously
}

//If ultra sonic sensor triggered, do this
void doAutonomousUltraSonicProcedure(){
  stopMotorsMS(1000); //Make sure that the mower has stopped

  calculateAndUpdateXAndYCoordinates(); //Send the current position to the Pi
  
  sendSerialUltraSonicTriggered(); //Back up, rotate, and continue moving autonomously
  
  waitForImageCaptured();

  doReverseProcedure();
}

//This function simply does a simple reversing manuever
void doReverseProcedure(){
  activateAutonomousLEDs();
  driveTime(1500, BACKWARD, MOTOR_SPEED_AUTONOMOUS_BACKWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  stopMotorsMS(1000);
  activateAutonomousLEDs();
  
  rotateByDegrees(random(110, 250), randomLeftOrRight(), MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT * PERCENTAGE_TO_PWM_FACTOR); 
  
  stopMotorsMS(random(1000, 3000));
  activateAutonomousLEDs();
}


/*
 * This function is called when the Arduino should wait for the Pi to take the picture through the camera.
 * The mower stand still and waits for an acknowledgement.
 * After a set time-out value, the mower continoues anyway.
 * 
 * For RNR5
 */
//void waitForImageCaptured(){
//  bool doLoop = true;
//  long timeToCapture = millis() + CAMERA_CAPTURE_TIME;
//  
//  while(doLoop){
//    stopMotorsMS(1000);
//    doSerialTick(false);  //We only want to read if any message is on the bus, we do not want to acknowledge it since the message should be an acknowledgment to begin with
//    if(recievedCaptureAck()){
//      doLoop = false;
//    }
//    else if(millis() > timeToCapture) {
//      Serial.println(getRecievedSerialDataFirstPart());
//      Serial.println("ERROR IN CAMERA CAPTURE");
//      doLoop = false;
//    }
//  }
//}

void waitForImageCaptured(){
  bool doLoop = true;
  long timeToCapture = millis() + CAMERA_CAPTURE_TIME;
  
  while(doLoop){
    stopMotorsMS(1000);
    doSerialTick();  //We only want to read if any message is on the bus, we do not want to acknowledge it since the message should be an acknowledgment to begin with
    if(getImageCaptured()){
      setImageCaptured(false);
      doLoop = false;
    }
    else if(millis() > timeToCapture) {
      setImageCaptured(false);
      doLoop = false;
    }
  }
}

void setImageCaptured(bool value){
  imageCaptured = value;
}

bool getImageCaptured(){
  return imageCaptured;
}


/*
 * 
 * DIAGNOSTIC CODE
 * 
 * The following functions test specific functions found in this module.
 * 
 */

int doAutonomousLineFollowerProcedureTest(){
  int errorCounter = 0;

  if(!resetStateLEDsTest()){errorCounter ++;}

  stopMotorsMS(1000);

  errorCounter += doReverseProcedureTest();

  return errorCounter;
}

int doReverseProcedureTest(){
  int errorCounter = 0;

  if(!resetStateLEDsTest()){errorCounter ++;}

  stopMotorsMS(1000);

  driveTime(1500, BACKWARD, MOTOR_SPEED_AUTONOMOUS_BACKWARD * PERCENTAGE_TO_PWM_FACTOR);

  if(!resetStateLEDsTest()){errorCounter ++;}

  stopMotorsMS(random(1000, 3000));
  
  rotateByDegrees(random(110, 250), randomLeftOrRight(), MOTOR_SPEED_AUTONOMOUS_RIGHT_OR_LEFT * PERCENTAGE_TO_PWM_FACTOR); 

  stopMotorsMS(random(1000, 3000));
  
  if(!resetStateLEDsTest()){errorCounter ++;}
  
  return errorCounter;
}

bool waitForImageCapturedTest(){
  bool goneThroughLoop = false;
  bool resetLedFailed = false;

  bool doLoop = true;
  long timeToCapture = millis() + CAMERA_CAPTURE_TIME;
  
  while(doLoop){
    inAutonomousDiagModuleLED();
    stopMotorsMS(1000);
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

  stopMotorsMS(1000);
  
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
  inAutonomousDiagModuleLED();
}

bool autonomousDiagnosticTestSuccess(){
  long timeToStopTest = millis() + TIME_TO_TEST_AUTONOMOUS_STATE_MS;
  int amountOfErrors = 0;

  while(millis() < timeToStopTest){
    amountOfErrors += doAutonomousTickTest();
  }

  stopMotorsMS(1000);

  if(amountOfErrors > 0){
    return false;
  }
  else{
    return true;
  }
}
