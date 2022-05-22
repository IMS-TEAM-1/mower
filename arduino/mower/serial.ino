/*
 * The following module contains code which handles the communication to and from the Pi.
 * It periodically reads the serial bus and stores what ever it recieves and then does the appropiate action depending on what it recieved.
 */

 /*
  * Code here is due to RNR4 and RNR16
  */


//Global variables used to store the latest messages recieved, time when the mower last got updated and how many missed messages we have missed
String recievedMessage;
long timeAtLastSerialUpdate;

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
void doSerialTick(){
  if(millis() - timeAtLastSerialUpdate > SERIAL_UPDATE_FREQUENCY_MS){
    timeAtLastSerialUpdate = millis();
    
    readSerialData();
    
    clearStoredMessages();
  }
}

////As long as there are data available on the serial bus, read and store it, then acknowelge it to the Pi
void readSerialData(){
  if(Serial.available() > 0){
    numberOfTicksMissed = 0;
    
    recievedMessage = readSerialBus();

    setSerialDataRecieved(recievedMessage);

    ackMessage(getSerialDataRecieved());

    Serial.flush();
    
  }
  else{
    numberOfTicksMissed++;
  }
}

//Simply adds "!" to a message if sucessful, "?" if not
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

void setSerialDataRecieved(String s_dataToStore){
  recievedMessage = s_dataToStore;
}

String getSerialDataRecieved(){
  return recievedMessage;
}

//Some specific "?", "C" and coorinate functions used often
void sendMessageNOK(String message){
  message.trim();
  Serial.println(message + "?");
}

void sendMessageAck(String message){
  message.trim();
  Serial.println(message + "!");
}

void sendSerialUltraSonicTriggered(){
  Serial.println("C");
}

void sendSerialCoordinates(){
  Serial.println("P" + String(getCoordinateX()) + "," + String(getCoordinateY()));
}

void sendDiagnosticSuccess(){
  Serial.println("D0");
}

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

void clearStoredMessages(){
  recievedMessage = "";
}

bool recievedCaptureAck(){
  if(getSerialDataRecieved().equals("C!")){
    return true;
  }
  else
    return false;
}

/*
 * The following code is used for decoding and using the messages recieved.
 * This is done with the help of switch cases and some enums.
 * Depening on what message is recieved, the mower should act accordingly.
 */
bool ackReviecedMessage(){
  switch(convertMessageToInt(getSerialDataRecieved())){
    case(Hello):
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(Standby):
      currentState = STANDBY;
      sendMessageAck(getSerialDataRecieved());
      //Reset map coordinates
      resetCoordinates();
      resetEncoderValues();
      return true;
    case(CaptureDone):
      setImageCaptured(true);
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
      resetCoordinates();
      resetEncoderValues();
      currentState = DIAGNOSTIC;
      sendMessageAck(getSerialDataRecieved());
      return true;
    case(Error):
      return false;
  }
}

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
  else if(message.equals("C!")){
    return CaptureDone;
  }
  else
    return Error;
}
