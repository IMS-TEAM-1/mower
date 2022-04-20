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
      currentState = STANDBY;
    }
  }
}

String getRecievedSerialDataFirstPart(){
  return recievedMessageFirstPart;
}

String getRecievedSerialDataSecondPart(){
  return recievedMessageSecondPart;
}

void checkRecievedMessage(char message[MAX_MESSAGE_LENGTH]){
  char temprecievedMessageFirstPart[MAX_MESSAGE_LENGTH];
  char temprecievedMessageSecondPart[MAX_MESSAGE_LENGTH];

  bool colonDetected = false;
  int temprecievedMessageFirstPartLen = 0;
  
  for(int i = 0; i < (sizeof(message) - 1); i++){
    if(message[i] == ':')
    {
      colonDetected = true;
      temprecievedMessageFirstPartLen = sizeof(temprecievedMessageFirstPart) - 1;
    }
    
    if(!colonDetected){
      temprecievedMessageFirstPart[i] = message[i];
    }
    else{
      temprecievedMessageSecondPart[(i - temprecievedMessageFirstPartLen)] = message[i];
    }
  }
  
  recievedMessageFirstPart = String(temprecievedMessageFirstPart);
  recievedMessageSecondPart = String(temprecievedMessageSecondPart);
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

bool ackMessage(String messageToAck){
  switch(convertMessageFirstPartToEnum(messageToAck)){
    case(Hello):
      sendMessageAck("Hello");
      return true;
    case(Standby):
      currentState = STANDBY;
      sendMessageAck("STANDBY");
      return true;
    case(Autonomous):
      currentState = AUTONOMOUS;
      sendMessageAck("AUTONOMOUS");
      return true;
    case(Manual):
      currentState = MANUAL;
      if(ackMessageSecondPart(getRecievedSerialDataSecondPart())){
        return true;
      }
      else{
        return false;
      }
    default:
      return false;
  }
}

bool ackMessageSecondPart(String messageToAck){
  switch(convertMessageSecondPartToEnum(messageToAck)){
    case(None):
      currentDirection = NONE;
      sendMessageAck("NONE");
      return true;
    case(Forward):
      currentDirection = FORWARD;
      sendMessageAck("FORWARD");
      return true;
    case(Backward):
      currentDirection = BACKWARD;
      sendMessageAck("BACKWARD");
      return true;
    case(Left):
      currentDirection = LEFT;
      sendMessageAck("LEFT");
      return true;;
    case(Right):
      currentDirection = RIGHT;
      sendMessageAck("RIGHT");
      return true;
    default:
      return false;
  }
}

messageFirstPart_t convertMessageFirstPartToEnum(String message){
  if(message.equals("MANUAL")){
    return Manual;
  }
  else if(message.equals("Hello")){
    return Hello;
  }
  else if(message.equals("STANDBY")){
    return Standby;
  }
  else if(message.equals("AUTONOMOUS")){
    return Autonomous;
  }
  else
    while(true) Serial.println("ERROR IN CONVERSION OF MESSAGE IN mower.ino");
}

messageSecondPart_t convertMessageSecondPartToEnum(String message){
  if(message.equals("FORWARD")){
    return Forward;
  }
  else if(message.equals("BACKWARD")){
    return Backward;
  }
  else if(message.equals("LEFT")){
    return Left;
  }
  else if(message.equals("RIGHT")){
    return Right;
  }
  if(message.equals("NONE")){
    return None;
  }
  else
    while(true) Serial.println("ERROR IN CONVERSION OF MESSAGE IN mower.ino");
}
