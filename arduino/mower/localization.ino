int coordinateX = 0;
int coordinateY = 0;

void doLocalizationTick(){
  if(millis() > timeForNextLocationTick){
    timeForNextLocationTick = timeForNextLocationTick + AUTONOMOUS_LOCATION_TICK_TIME_MS;

    calculateAndUpdateXAndYCoordinates();

    sendSerialCoordinates();
  }
}

void calculateAndUpdateXAndYCoordinates(){
  stopMotors();
  _delay(0.3); //To make sure that robot is stopped

  float newXCoordinate;
  float newYCoordinate;
  
  setGyroValueAtEnd(getGyroZ());
  
  float calcTemp = ((getAverageGyroValue() + LOCALIZATION_CIRCLE_ROTATION_OFFSET) * DEGREES_TO_RADIAN_FACTOR);

  newYCoordinate = getCoordinateY() + (getDistanceTravelled() * sin(calcTemp));
  newXCoordinate = getCoordinateX() + (getDistanceTravelled() * (cos(calcTemp) * -1));

//  if(getAverageGyroValue() <= 90 && getAverageGyroValue() > -90){
//    newYCoordinate = getCoordinateY() + (getDistanceTravelled() * sin(calcTemp));
//  }
//  else if((getAverageGyroValue() > 90 && getAverageGyroValue() <= 180) || (getAverageGyroValue() >= -180 && getAverageGyroValue() <= -90)){
//    newYCoordinate = getCoordinateY() + (getDistanceTravelled() * sin(calcTemp));
//  }
//
//  if(getAverageGyroValue() <= 180 && getAverageGyroValue() >= 0){
//    newXCoordinate = getCoordinateX() + (getDistanceTravelled() * cos(calcTemp));
//  }
//  else if(getAverageGyroValue() >= -180 && getAverageGyroValue() < 0){
//    newXCoordinate = getCoordinateX() - (getDistanceTravelled() * cos(calcTemp));
//  }

//  Serial.println("gyroAverage: " + String(getAverageGyroValue()));
//  Serial.println("\ncalcTemp: " + String(calcTemp));
//  Serial.println("Cos: " + String(cos(calcTemp)));
//  Serial.println("Sin: " + String(sin(calcTemp)));

  setCoordinateX(newXCoordinate);
  setCoordinateY(newYCoordinate);

  resetEncoderValues();
}

float getDistanceTravelled(){
  return (getEncoderAverage() * MILLIMETER_PER_ENCOER_PULSE);
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
