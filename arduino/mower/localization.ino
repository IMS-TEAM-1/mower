int coordinateX = 0;
int coordinateY = 0;

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
