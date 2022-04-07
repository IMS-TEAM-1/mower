void setupSerial(){
  Serial.begin(9600); //Initiate serial communication
}

void getSerialData(){
  if (millis() - timeAtLastUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    timeAtLastUpdate = millis();
    Serial.println("update");
  }
}
