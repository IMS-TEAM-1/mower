String recievedMessageFirstPart;
String recievedMessageSecondPart;
float timeAtLastSerialUpdate = 0;

const char* delimiter = ":";

void setupSerial(){
  Serial.begin(115200); //Initiate serial communication
}

void readSerialData(){
  if (millis() - timeAtLastSerialUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    timeAtLastSerialUpdate = millis();
    
    if(Serial.available() > 0){
      String message = readSerialBus();
      
      checkAndSetRecievedMessage(message);

      ackMessage(message);
    }
    else{
      //currentState = STANDBY;
    }
  }
}

void ackMessage(String message){
  if(!ackReviecedMessage()){
    sendMessageNOK(message);
  }
}

String readSerialBus(){
  String message;
  while (Serial.available() > 0) {
    message += (char)Serial.read();
  }
  return message;
}

String getRecievedSerialDataFirstPart(){
  return recievedMessageFirstPart;
}

String getRecievedSerialDataSecondPart(){
  return recievedMessageSecondPart;
}

void checkAndSetRecievedMessage(String message){
  char* token;
  char buf[message.length()];

  message.toCharArray(buf, message.length());
  token = strtok(buf, delimiter);
  
  recievedMessageFirstPart = token;
  if(token != NULL){
    token = strtok(NULL, delimiter);
    recievedMessageSecondPart = token;
  }
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

bool ackReviecedMessage(){
  if(ackReviecedMessageFirstPart()){
    return true;
  }
  else{
    return false;
  }
}
