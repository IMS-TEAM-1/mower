String recievedMessage1;
String recievedMessage2;

void setupSerial(){
  Serial.begin(9600); //Initiate serial communication
}

void readSerialData(){
  if (millis() - timeAtLastUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    timeAtLastUpdate = millis();
    //Serial.println("update");
    
    if(Serial.available() > 0){
      while (Serial.available() > 0) {
        //Create a place to hold the incoming message
        static char message[MAX_MESSAGE_LENGTH];
        static unsigned int message_pos = 0;
      
        //Read the next available byte in the serial receive buffer
        char inByte = Serial.read();
     
        //Message coming in (check not terminating character) and guard for over message size
        
        if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) ) {
          //Add the incoming byte to our message
          message[message_pos] = inByte;
          message_pos++;
        }
        //Full message received...
        else {
          //Add null character to string
          message[message_pos] = '\0';
      
          checkRecievedMessage(message);
      
          //Reset for the next message
          message_pos = 0;
        }
      }
    }
    else{
      Serial.println("NO COMMUNICATION");
      currentState = STANDBY;
    }
  }
}

String getRecievedSerialData1(){
  return recievedMessage1;
}

String getRecievedSerialData2(){
  return recievedMessage2;
}

void checkRecievedMessage(char message[MAX_MESSAGE_LENGTH]){
  char tempRecievedMessage1[MAX_MESSAGE_LENGTH];
  char tempRecievedMessage2[MAX_MESSAGE_LENGTH];

  bool colonDetected = false;
  int tempRecievedMessage1Len = 0;
  
  for(int i = 0; i < (sizeof(message) - 1); i++){
    if(message[i] == ":")
    {
      colonDetected = true;
      tempRecievedMessage1Len = sizeof(tempRecievedMessage1) - 1;
    }
    
    if(!colonDetected){
      tempRecievedMessage1[i] = message[i];
    }
    else{
      tempRecievedMessage2[(i - tempRecievedMessage1Len)] = message[i];
    }
  }
  
  recievedMessage1 = String(tempRecievedMessage1);
  recievedMessage2 = String(tempRecievedMessage2);
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
  if(recievedMessage1 == "CAPTURE" && recievedMessage2 == "ack"){
    return true;
  }
  else
    return false;
}
