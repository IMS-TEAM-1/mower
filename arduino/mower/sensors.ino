bool getUltraSonicSensorTriggered(){
  Serial.println("Ultrasonic sensor triggered");
  return (ultraSonicSensor.distanceCm() < ULTRA_SONIC_SENSOR_DISTANCE_CM);
}
  
bool getLineFollowerTriggered(){
  Serial.println("IR sensor triggered");
  return (lineFollowerSensor.readSensors() == 0.000000);
}
