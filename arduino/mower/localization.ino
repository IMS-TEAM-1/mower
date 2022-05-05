int coordinateX = 0;
int coordinateY = 0;

bool TESTfirstTapeFound = false;
bool TESTsecondTapeFound = false;

void doDrivingInASquareTest(){
  driveForwardDistance(500);
      
  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);
  

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  _delay(3);

  driveForwardDistance(500);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  _delay(3);

  driveForwardDistance(500);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, LEFT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);

  

  _delay(3);

  driveForwardDistance(500);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);



  rotateByDegrees(180, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveForwardDistance(500);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveForwardDistance(500);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveForwardDistance(500);

  calculateAndUpdateXAndYCoordinates();
  
  sendSerialCoordinates();

  _delay(3);

  rotateByDegrees(90, RIGHT, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  
  _delay(3);

  driveForwardDistance(500);

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

    driveForwardDistance(1000);

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

void driveForwardDistance(int millimeters){
  resetEncoderValues();

  while((getDistanceTravelled() < millimeters - MILLIMETER_DISTANCE_WHEN_FREE_ROLLING_AFTER_FULL_SPEED)){
    move(FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR);
  }
  Serial.println(getGyroZ());
  delay(300);
}

void calculateAndUpdateXAndYCoordinates(){
  setGyroValueAtEnd(getGyroZ());
  
  float calcTemp = ((getAverageGyroValue() + LOCALIZATION_CIRCLE_ROTATION_OFFSET) * DEGREES_TO_RADIAN_FACTOR);
  
  float newXCoordinate =  getCoordinateX() + (getDistanceTravelled() * cos(calcTemp) * -1);
  float newYCoordinate = getCoordinateY() + (getDistanceTravelled() * sin(calcTemp));

  Serial.println("gyroAverage: " + String(getAverageGyroValue()));
  Serial.println("\ncalcTemp: " + String(calcTemp));
  Serial.println("Cos: " + String(cos(calcTemp)));
  Serial.println("Sin: " + String(sin(calcTemp)));

  setCoordinateX(newXCoordinate);
  setCoordinateY(newYCoordinate);

  resetEncoderValues();
}

float getGyroX(){
  return gyro.getAngle(1);
}

float getGyroY(){
  return gyro.getAngle(2);
}

float getGyroZ(){
  return gyro.getAngle(3);
}

void gyroPrintValues(){
  Serial.println("Gyro X: " + String(getGyroX()));
  Serial.println("Gyro Y: " + String(getGyroY()));
  Serial.println("Gyro Z: " + String(getGyroZ()) + "\n");
}

void printCoordinates(){
  Serial.println("Coordinate X: " + String(getCoordinateX()));
  Serial.println("Coordinate Y: " + String(getCoordinateY()) + "\n");
}

int getCoordinateX(){
  return coordinateX;
}

int getCoordinateY(){
  return coordinateY;
}

void setCoordinateX(int value){
  coordinateX = value;
}

void setCoordinateY(int value){
  coordinateY = value;
}

void resetCoorinateX(){
  coordinateX = 0;
}

void resetCoorinateY(){
  coordinateY = 0;
}

void resetCoordinates(){
  resetCoorinateX();
  resetCoorinateY();
}
