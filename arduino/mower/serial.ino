String recievedMessageFirstPart;
String recievedMessageSecondPart;
float timeAtLastSerialUpdate = 0;

void setupSerial(){
  Serial.begin(115200); //Initiate serial communication
}

void readSerialData(){
  if (millis() - timeAtLastSerialUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    timeAtLastSerialUpdate = millis();
    //Serial.println("update");
    String message;
    
    if(Serial.available() > 0){
      while (Serial.available() > 0) {
        //Read the next available byte in the serial receive buffer
        message += (char)Serial.read();
      }
      Serial.flush();
      
      checkAndSetRecievedMessage(message);
  
      if(ackReviecedMessage()){
        return;
      }
      else{
        Serial.println("fail");
      }
    }
    else{
      currentState = STANDBY;
    }
  }
}

String getRecievedSerialDataFirstPart(){
  //Serial.println("First part: " + recievedMessageFirstPart);
  return recievedMessageFirstPart;
}

String getRecievedSerialDataSecondPart(){
  //Serial.println("First part: " + recievedMessageSecondPart);
  return recievedMessageSecondPart;
}

void checkAndSetRecievedMessage(String message){
  char temprecievedMessageFirstPart[message.length()-1];
  char temprecievedMessageSecondPart[message.length()-1];

  char tempMessageChar[message.length()];
  message.toCharArray(tempMessageChar, message.length());
  
  bool colonDetected = false;

  for(int i = 0; i < (message.length() - 1); i++){
    if(tempMessageChar[i] == ':')
    {
      colonDetected = true;
    }
    
    if(!colonDetected){
      temprecievedMessageFirstPart[i] = tempMessageChar[i];
    }
    else{
      int q = i - sizeof(temprecievedMessageFirstPart);
      temprecievedMessageSecondPart[q] = tempMessageChar[i];
    }
  }
  recievedMessageFirstPart = (String)temprecievedMessageFirstPart;
  recievedMessageSecondPart = (String)temprecievedMessageSecondPart;
}

void sendMessageNOK(String message){
  Serial.println(message + ":" + "nok");
}

void sendMessageAck(String message){
  Serial.println(message + ":" + "ack");
}

void sendSerialUltraSonicTriggered(){
  Serial.println("CAPTURE");
}

bool recievedCaptureAck(){
  if(recievedMessageFirstPart == "CAPTURE" && recievedMessageSecondPart == "ack"){
    return true;
  }
  else
    return false;
}
