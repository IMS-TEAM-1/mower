float gyroValueAtStart = 0;
float gyroValueAtEnd = 0;

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

void setGyroValueAtStart(float value){
  gyroValueAtStart = value;
}

void setGyroValueAtEnd(float value){
  gyroValueAtEnd = value;
}

void resetGyroValueAtStart(){
  setGyroValueAtStart(getGyroZ());
}

void resetGyroValueAtEnd(){
  setGyroValueAtEnd(getGyroZ());
}

void resetGyroStartAndEnd(){
  resetGyroValueAtStart();
  resetGyroValueAtEnd();
}

float getGyroValueAtStart(){
  return gyroValueAtStart;
}

float getGyroValueAtEnd(){
  return gyroValueAtEnd;
}

float getAverageGyroValue(){
  return ((getGyroValueAtStart() + getGyroValueAtEnd()) * 0.5);
}
