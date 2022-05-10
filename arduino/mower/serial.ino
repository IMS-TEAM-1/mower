//Global variables used to store the latest messages recieved, time when the mower last got updated and how many missed messages we have missed
String recievedMessage;
String recievedMessageFirstPart;
String recievedMessageSecondPart;
long timeAtLastSerialUpdate;
int numberOfTicksMissed = 0;

//By changing this variable, it changes what character the serial communication sees as a character between two separate words in one string
const char* delimiter = ":";

//Initiate serial communication
void setupSerial(){
  Serial.begin(115200);
}

/*
 * This is the main function of the serial comunication.
 * When called, it updates what the serial monitor recieves periodically.
 * The update frequency can be changed in config.h.
 * 
 * If alot of messages are missed, then change to stanby state.
 * This ensures that the mower does not move when it does not recieve any commands.
 * The amount of max missed messages can be fine tuned in config.h is the mower does not move smooth when manually controlling it.
 * 
 * The functions used build on each other, and goes deep into other functions.
 * Therefore, it may be confusing to read.
 * It can definatly be developed in a better way, but it works for now according to our designed protocol.
 */
void doSerialTick(bool ack){
  if(millis() - timeAtLastSerialUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    if(numberOfTicksMissed > MAX_ALLOWED_MISSED_SERIAL_TICKS){
      currentState = STANDBY;
      //Otherwise, the mower get locked
      numberOfTicksMissed = 0;
    }
    else{
      timeAtLastSerialUpdate = millis();
      
      readSerialData(ack);

      //Should be removed when in final use, only here for when testing through Arduino monitor
      numberOfTicksMissed = 0;
    }
  }
}

//As long as there are data available on the serial bus, read and store it, then acknowelge it to the Pi
void readSerialData(bool ack){
  if(Serial.available() > 0){
    numberOfTicksMissed = 0;
    
    recievedMessage = readSerialBus();

    checkAndSetRecievedMessage(recievedMessage);

    if(ack){
      ackMessage(recievedMessage);
    }

    Serial.flush();
    
  }
  else{
    numberOfTicksMissed++;
  }
}

/*
 * The following function is used to store the separate messages within one string
 * How this currently works is that it can either recieve one or two messages in one string.
 * This is done by sending ONE string which contains TWO word by SEPARATING them with the "delimiter" (seen above).
 * 
 * Example: Serial.println("MANUAL:FORWARD") -> First word: "MANUAL", second word: "FORWARD"
 */
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

//Simply adds ":ack" to a message if sucessful, ":nok" if not
void ackMessage(String message){
  if(!ackReviecedMessage()){
    sendMessageNOK(message);
  }
}

//As long as there are data to read, read it. Also, making sure that newline character is found
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

//Get the first word stored
String getRecievedSerialDataFirstPart(){
  return recievedMessageFirstPart;
}

//Get the second word stored
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

void sendSerialCoordinates(){
  Serial.println("POS:" + String(getCoordinateX()) + "," + String(getCoordinateY()));
}

void clearMessages(){
  recievedMessage = "";
  recievedMessageFirstPart = "";
  recievedMessageSecondPart = "";
}

//Used in autonomous to make sure that the mower has recieved acks on that the picture has been taken
bool recievedCaptureAck(){
  if(getRecievedSerialDataFirstPart().equals("CAPTURE") && getRecievedSerialDataSecondPart().equals("ack")){
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

/*
 * The following code is used for decoding and using the messages recieved.
 * This is done with the help of switch cases and some enums.
 * Depening on what message is recieved, and if it is the first or second word, the mower should act accordingly.
 */
bool ackReviecedMessageFirstPart(){
  switch(convertMessageFirstPartToInt(getRecievedSerialDataFirstPart())){
    case(Manual):
      if(ackReviecedMessageSecondPart()){
        currentState = MANUAL;
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
