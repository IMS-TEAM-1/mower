int coordinateX = 0;
int coordinateY = 0;

bool TESTfirstTapeFound = false;
bool TESTsecondTapeFound = false;

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

    move(FORWARD, MAX_MOTOR_SPEED);
    _delay(0.5);
    
    while(!TESTsecondTapeFound){
      move(FORWARD, MAX_MOTOR_SPEED);
      if(getLineFollowerTriggered()){
        TESTsecondTapeFound = true; 
      }
    }

    printEncoderPulseValues();

    Serial.println("\nDistance travelled: ");

    Serial.println(getDistanceTravelled());
    
    while(true){
      stopMotors();
    }
  }
}

float gyroGetX(){
  return gyro.getAngle(1);
}

float gyroGetY(){
  return gyro.getAngle(2);
}

float gyroGetZ(){
  return gyro.getAngle(3);
}

void gyroPrintValues(){
  Serial.println("Gyro X: " + String(gyroGetX()));
  Serial.println("Gyro Y: " + String(gyroGetY()));
  Serial.println("Gyro Z: " + String(gyroGetZ()) + "\n");
}

int getCoordinateX(){
  return coordinateX;
}

int getCoordinateY(){
  return coordinateX;
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
