/*
 * This file contains testing code used when developing the robot.
 * Most of the code has been deleted over time but here are some of the last examples used to test some functionalities.
 * Do not mistake this file for final testing of the mower
 */

bool TESTfirstTapeFound = false;
bool TESTsecondTapeFound = false;

void doDrivingInASquareTest(){
  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
      
  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);
  

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  _delay(3);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  _delay(3);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  _delay(3);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);



  rotateByDegrees(180, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveDistance(500, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);
  
  
  while(true){
    stopMotors();
  }
}

void testSpeedOfRobot(){
  if(!getLineFollowerTriggered()){
    Serial.println("Moving until tape found");

    move(FORWARD, MAX_MOTOR_SPEED);
  }
  else{
    TESTfirstTapeFound = true;
    Serial.println("Tape found, stopping in 3 seconds");
    stopMotors();
    resetEncoderValues();
    _delay(3);

    //move(FORWARD, MAX_MOTOR_SPEED);
    //_delay(0.5);
    
//    while(!TESTsecondTapeFound){
//      move(FORWARD, MAX_MOTOR_SPEED);
//      if(getLineFollowerTriggered()){
//        TESTsecondTapeFound = true; 
//      }
//    }

    driveDistance(1000, FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

    calculateAndUpdateXAndYCoordinates();
    printCoordinates();
    
    //printEncoderPulseValues();
    //Serial.println("\nDistance travelled: ");
    //Serial.println(getDistanceTravelled());
    
    while(true){
      stopMotors();
    }
  }
}

void doRotationTest(){
  rotateByDegrees(90, LEFT, MAX_MOTOR_SPEED);

  _delay(3);

  rotateByDegrees(180, RIGHT, MAX_MOTOR_SPEED);

  _delay(3);

  rotateByDegrees(270, RIGHT, MAX_MOTOR_SPEED);

  _delay(3);

  rotateByDegrees(360, LEFT, MAX_MOTOR_SPEED);
}
