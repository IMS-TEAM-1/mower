//Used for collision control
bool getUltraSonicSensorTriggered(){
  return (ultraSonicSensor.distanceCm() < ULTRA_SONIC_SENSOR_DISTANCE_CM);
}

//Used to find black tape
bool getLineFollowerTriggered(){
  return (lineFollowerSensor.readSensors() == 0.000000);
}
