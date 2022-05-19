/*
 * The following module contains code which handles the communication to and from the Pi.
 * It periodically reads the serial bus and stores what ever it recieves and then does the appropiate action depending on what it recieved.
 */

 /*
  * Code here is due to RNR4 and RNR16
  */


//Global variables used to store the latest messages recieved, time when the mower last got updated and how many missed messages we have missed
String recievedMessage;
//String recievedMessageFirstPart;
//String recievedMessageSecondPart;
long timeAtLastSerialUpdate;
int numberOfTicksMissed = 0;

//By changing this variable, it changes what character the serial communication sees as a character between two separate words in one string
//const char* delimiter = ":";

//Initiate serial communication
void setupSerial(){
  Serial.begin(57600); //Changed from 115200 due to unstable connection between Arduino and Pi
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
  clearStoredMessages();
}

////As long as there are data available on the serial bus, read and store it, then acknowelge it to the Pi
//void readSerialData(bool ack){
//  if(Serial.available() > 0){
//    numberOfTicksMissed = 0;
//    
//    recievedMessage = readSerialBus();
//
//    checkAndSetRecievedMessage(recievedMessage);
//
//    if(ack){
//      ackMessage(recievedMessage);
//    }
//
//    Serial.flush();
//    
//  }
//  else{
//    numberOfTicksMissed++;
//  }
//}

void readSerialData(bool ack){
  if(Serial.available() > 0){
    numberOfTicksMissed = 0;
    
    recievedMessage = readSerialBus();

    setSerialDataRecieved(recievedMessage);

    if(ack){
      ackMessage(getSerialDataRecieved());
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
//void checkAndSetRecievedMessage(String message){
//  char* token;
//  char buf[message.length()+1];
//
//  message.toCharArray(buf, message.length() + 1);
//  token = strtok(buf, delimiter);
//  
//  setSerialDataFirstPart(token);
//  
//  if(token != NULL){
//    token = strtok(NULL, delimiter);
//    setSerialDataSecondPart(token);
//  }
//}

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

////Set the first word stored
//void setSerialDataFirstPart(String s_dataToStore){
//  recievedMessageFirstPart = s_dataToStore;
//}
//
////Set the second word stored
//void setSerialDataSecondPart(String s_dataToStore){
//  recievedMessageSecondPart = s_dataToStore;
//}
//
////Get the first word stored
//String getRecievedSerialDataFirstPart(){
//  return recievedMessageFirstPart;
//}
//
////Get the second word stored
//String getRecievedSerialDataSecondPart(){
//  return recievedMessageSecondPart;
//}

void setSerialDataRecieved(String s_dataToStore){
  recievedMessage = s_dataToStore;
}

String getSerialDataRecieved(){
  return recievedMessage;
}

////Some specific NOK, CAPTURE and coorinate functions used often
//void sendMessageNOK(String message){
//  message.trim();
//  Serial.println(message + ":" + "nok");
//}
//
//void sendMessageAck(String message){
//  message.trim();
//  Serial.println(message + ":" + "ack");
//}

//Some specific NOK, CAPTURE and coorinate functions used often
void sendMessageNOK(String message){
  message.trim();
  Serial.println(message + "?");
}

void sendMessageAck(String message){
  message.trim();
  Serial.println(message + "!");
}

//void sendSerialUltraSonicTriggered(){
//  Serial.println("CAPTURE");
//}

void sendSerialUltraSonicTriggered(){
  Serial.println("C");
}

//void sendSerialCoordinates(){
//  Serial.println("POS:" + String(getCoordinateX()) + "," + String(getCoordinateY()));
//}

void sendSerialCoordinates(){
  Serial.println("P" + String(getCoordinateX()) + "," + String(getCoordinateY()));
}

//void sendDiagnosticSuccess(){
//  Serial.println("DIAGNOSTIC:ok");
//}

void sendDiagnosticSuccess(){
  Serial.println("D0");
}

//Convert all fault codes from queue into one string and send it to Pi
//void sendDiagnosticFailed(ArduinoQueue<int> faultCodes){
//  String s_faultCodes;
//  
//  while(!faultCodes.isEmpty()){
//    s_faultCodes += String(faultCodes.dequeue());
//    if(!faultCodes.isEmpty()){
//      s_faultCodes += ",";
//    }
//  }
//  
//  Serial.println("DIAGNOSTIC:" + s_faultCodes);
//}

void sendDiagnosticFailed(ArduinoQueue<int> faultCodes){
  String s_faultCodes;
  
  while(!faultCodes.isEmpty()){
    s_faultCodes += String(faultCodes.dequeue());
    if(!faultCodes.isEmpty()){
      s_faultCodes += ",";
    }
  }
  
  Serial.println("D" + s_faultCodes);
}

//Resets all stored messages
//void clearStoredMessages(){
//  recievedMessage = "";
//  recievedMessageFirstPart = "";
//  recievedMessageSecondPart = "";
//}

void clearStoredMessages(){
  recievedMessage = "";
}


//Used in autonomous to make sure that the mower has recieved acks on that the picture has been taken
//bool recievedCaptureAck(){
//  if(getRecievedSerialDataFirstPart().equals("CAPTURE") && getRecievedSerialDataSecondPart().equals("ack")){
//    return true;
//  }
//  else
//    return false;
//}

bool recievedCaptureAck(){
  if(getSerialDataRecieved().equals("C!")){
    return true;
  }
  else
    return false;
}

////This function tries to ack the first message first and internalyy, if there is a second "parameter" in the word, it does that too
//bool ackReviecedMessage(){
//  if(ackReviecedMessageFirstPart()){
//    return true;
//  }
//  else{
//    return false;
//  }
//}

//bool ackReviecedMessage(){
//  if(ackReviecedMessageFirstPart()){
//    return true;
//  }
//  else{
//    return false;
//  }
//}

/*
 * The following code is used for decoding and using the messages recieved.
 * This is done with the help of switch cases and some enums.
 * Depening on what message is recieved, and if it is the first or second word, the mower should act accordingly.
 */
//bool ackReviecedMessageFirstPart(){
//  switch(convertMessageFirstPartToInt(getRecievedSerialDataFirstPart())){
//    case(Manual):
//      if(ackReviecedMessageSecondPart()){
//        currentState = MANUAL;
//        return true;
//      }
//      else{
//        return false;
//      }
//    case(Hello):
//      sendMessageAck("Hello");
//      return true;
//    case(Stop):
//      currentState = STANDBY;
//      sendMessageAck("STANDBY");
//      return true;
//    case(Autonomous):
//      currentState = AUTONOMOUS;
//      sendMessageAck("AUTONOMOUS");
//      return true;
//    case(Diagnostic):
//      currentState = DIAGNOSTIC;
//      sendMessageAck("DIAGNOSTIC");
//      return true;
//    case(Error_1):
//      return false;
//  }
//}
//
//bool ackReviecedMessageSecondPart(){
//  switch(convertMessageSecondPartToInt(getRecievedSerialDataSecondPart())){
//    case(None):
//      setCurrentDirection(NONE);
//      sendMessageAck("NONE");
//      return true;
//    case(Forward):
//      setCurrentDirection(FORWARD);
//      sendMessageAck("FORWARD");
//      return true;
//    case(Backward):
//      setCurrentDirection(BACKWARD);
//      sendMessageAck("BACKWARD");
//      return true;
//    case(Left):
//      setCurrentDirection(LEFT);
//      sendMessageAck("LEFT");
//      return true;
//    case(Right):
//      setCurrentDirection(RIGHT);
//      sendMessageAck("RIGHT");
//      return true;
//    case(Error_2):
//      return false;
//  }
//}

bool ackReviecedMessage(){
  switch(convertMessageToInt(getSerialDataRecieved())){
    case(Hello):
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(Standby):
      currentState = STANDBY;
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualStop):
      currentState = MANUAL;
      setCurrentDirection(NONE);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualForward):
      currentState = MANUAL;
      setCurrentDirection(FORWARD);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualBackward):
      currentState = MANUAL;
      setCurrentDirection(BACKWARD);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualLeft):
      currentState = MANUAL;
      setCurrentDirection(LEFT);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(ManualRight):
      currentState = MANUAL;
      setCurrentDirection(RIGHT);
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(Autonomous):
      currentState = AUTONOMOUS;
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(Diagnostic):
      currentState = DIAGNOSTIC;
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(Error):
      return false;
  }
}



////This is done to make the code easier to read within the case-switches
//messageFirstPart_t convertMessageFirstPartToInt(String message){
//  message.trim();
//  
//  if(message.equals("Hello")){
//    return Hello;
//  }
//  else if(message.equals("STANDBY")){
//    return Stop;
//  }
//  else if(message.equals("AUTONOMOUS")){
//    return Autonomous;
//  }
//  else if(message.equals("MANUAL")){
//    return Manual;
//  }
//  else if(message.equals("DIAGNOSTIC")){
//    return Diagnostic;
//  }
//  else
//    return Error_1;
//}
//
////This is done to make the code easier to read within the case-switches
//messageSecondPart_t convertMessageSecondPartToInt(String message){
//  message.trim();
//  
//  if(message.equals("NONE")){
//    return None;
//  }
//  else if(message.equals("FORWARD")){
//    return Forward;
//  }
//  else if(message.equals("BACKWARD")){
//    return Backward;
//  }
//  else if(message.equals("LEFT")){
//    return Left;
//  }
//  else if(message.equals("RIGHT")){
//    return Right;
//  }
//  else
//    return Error_2;
//}

messageRecieved_t convertMessageToInt(String message){
  message.trim();
  
  if(message.equals("H")){
    return Hello;
  }
  else if(message.equals("0")){
    return ManualStop;
  }
  else if(message.equals("1")){
    return ManualForward;
  }
  else if(message.equals("2")){
    return ManualRight;
  }
  else if(message.equals("3")){
    return ManualBackward;
  }
  else if(message.equals("4")){
    return ManualLeft;
  }
  else if(message.equals("S")){
    return Standby;
  }
  else if(message.equals("A")){
    return Autonomous;
  }
  else if(message.equals("D")){
    return Diagnostic;
  }
  else
    return Error;
}
