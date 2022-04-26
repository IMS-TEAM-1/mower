String recievedMessageFirstPart;
String recievedMessageSecondPart;
long timeAtLastSerialUpdate;
int numberOfTicksMissed = 0;

const char* delimiter = ":";

void setupSerial(){
  Serial.begin(115200); //Initiate serial communication
}

void doSerialTick(){
  if(millis() - timeAtLastSerialUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    if(numberOfTicksMissed > MAX_ALLOWED_MISSED_SERIAL_TICKS){
      currentState = STANDBY;
      numberOfTicksMissed = 0;
    }
    else{
      readAndAckSerialData();
      numberOfTicksMissed = 0;
    }
  }
}

void readAndAckSerialData(){
  timeAtLastSerialUpdate = millis();
  
  if(Serial.available() > 0){
    numberOfTicksMissed = 0;
    
    String message = readSerialBus();
    
    checkAndSetRecievedMessage(message);

    ackMessage(message);
  }
  else{
    numberOfTicksMissed++;
  }
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

void ackMessage(String message){
  if(!ackReviecedMessage()){
    sendMessageNOK(message);
  }
}

String readSerialBus(){
  String message;
  bool EoLFound = false;
  while (Serial.available() > 0) {
    message += (char)Serial.read();
    if(message.indexOf("\n") > 0){
      EoLFound = true;
    }
  }
  if(EoLFound){
    return message;
  }
  else{
    return "EoL Not found";
  }
}

String getRecievedSerialDataFirstPart(){
  return recievedMessageFirstPart;
}

String getRecievedSerialDataSecondPart(){
  return recievedMessageSecondPart;
}

void sendMessageNOK(String message){
  message.trim();
  Serial.println(message + ":" + "nok");
}

void sendMessageAck(String message){
  message.trim();
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

bool ackReviecedMessageFirstPart(){
  switch(convertMessageFirstPartToInt(getRecievedSerialDataFirstPart())){
    case(Manual):
      currentState = MANUAL;
      if(ackReviecedMessageSecondPart()){
        return true;
      }
      else{
        return false;
      }
    case(Hello):
      sendMessageAck("Hello");
      return true;
    case(Stop):
      currentState = STANDBY;
      sendMessageAck("STANDBY");
      return true;
    case(Autonomous):
      currentState = AUTONOMOUS;
      sendMessageAck("AUTONOMOUS");
      return true;
    case(Error_1):
      return false;
  }
}

bool ackReviecedMessageSecondPart(){
  switch(convertMessageSecondPartToInt(getRecievedSerialDataSecondPart())){
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
      return true;
    case(Right):
      currentDirection = RIGHT;
      sendMessageAck("RIGHT");
      return true;
    case(Error_2):
      return false;
  }
}

messageFirstPart_t convertMessageFirstPartToInt(String message){
  message.trim();
  
  if(message.equals("Hello")){
    return Hello;
  }
  else if(message.equals("STANDBY")){
    return Stop;
  }
  else if(message.equals("AUTONOMOUS")){
    return Autonomous;
  }
  else if(message.equals("MANUAL")){
    return Manual;
  }
  else
    return Error_1;
}

messageSecondPart_t convertMessageSecondPartToInt(String message){
  message.trim();
  
  if(message.equals("NONE")){
    return None;
  }
  else if(message.equals("FORWARD")){
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
  else
    return Error_2;
}
