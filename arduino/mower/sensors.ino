bool getUltraSonicSensorTriggered(){
  return (ultraSonicSensor.distanceCm() < ULTRA_SONIC_SENSOR_DISTANCE_CM);
}
  
bool getLineFollowerTriggered(){
  return (lineFollowerSensor.readSensors() == 0.000000);
}
