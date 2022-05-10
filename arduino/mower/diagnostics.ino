/*
 * This function is ran when the state of the mower is set to "diagnostic".
 * It checks the vital parts of the mower system and returns the functions that failes (if any did)
 * It firsts acks the "DIAGNOSTIC"-message to confirm the Pi that the mower is indeed running a diagnostics.
 * Afterwards, it does testing of each module and returns a string designed as "DIAGNOSTIC:good" if everythis is well.
 * Or, it returns something like "DIAGNOSTIC:1,3,7" if module 1, 3 and 7 was malfunctioning.
 */

diagntosicModule_t moduleCurrentlyDiagnosing = Diag_Main;
ArduinoQueue<int> storedFaultCodes(MAX_AMOUNT_OF_FAULT_CODES);

void doSelfDiagnosticTest(){
  moduleCurrentlyDiagnosing = Diag_Main;
  while(!storedFaultCodes.isEmpty()){
    storedFaultCodes.dequeue();
  }
  
  while(currentState == DIAGNOSTIC){
    resetStateLEDsTest();
    switch(moduleCurrentlyDiagnosing){
      case(Diag_Main):
        inDiagModuleLED(1);
        doMainDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Autonomous;
        break;
        
      case(Diag_Autonomous):
        inDiagModuleLED(2);
        doAutonomousDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Encoder;
        break;
        
      case(Diag_Encoder):
        inDiagModuleLED(3);
        doEncoderDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Gyro;
        break;
        
      case(Diag_Gyro):
        inDiagModuleLED(4);
        doGyroDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Joystick;
        break;
        
      case(Diag_Joystick):
        inDiagModuleLED(5);
        doJoystickDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Localization;
        break;
        
      case(Diag_Localization):
        inDiagModuleLED(6);
        doLocalizationDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Motorcontrol;
        break;
        
      case(Diag_Motorcontrol):
        inDiagModuleLED(7);
        doMotorcontrolDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Sensors;
        break;
        
      case(Diag_Sensors):
        inDiagModuleLED(8);
        doSensorsDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_SerialCom;
        break;
        
      case(Diag_SerialCom):
        inDiagModuleLED(9);
        doSerialComDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Done;
        break;
        
       case(Diag_Done):
        if(storedFaultCodes.isEmpty()){
          sendDiagnosticSuccess();
        }
        else{
          sendDiagnosticFailed(storedFaultCodes);
        }
        moduleCurrentlyDiagnosing = Diag_Main;
        currentState = STANDBY;
        break;
      }
   }
}

//This test specifically is unneccessary since we already are in the main code and know that it works, therefore, we do a simple if
void doMainDiag(){
  bool moduleSucceeded = false;

  if(currentState == DIAGNOSTIC){
    moduleSucceeded = true;
  }
  
  if(!moduleSucceeded){storedFaultCodes.enqueue(1);}
}

void doAutonomousDiag(){
  bool moduleSucceeded = false;

  moduleSucceeded = autonomousDiagnosticTestSuccess();
  
  if(!moduleSucceeded){storedFaultCodes.enqueue(2);}
}

void doEncoderDiag(){
  bool moduleSucceeded = false;

  resetEncoderValues();

  if(getEncoder1Pulses() == 0 && getEncoder2Pulses() == 0){
    int randomValue = random(0, 1000);
    
    setEncoder1Pulse(randomValue);
    setEncoder2Pulse(randomValue);

    if(getEncoder1Pulses() == randomValue && getEncoder2Pulses() == randomValue){
      moduleSucceeded = true;
    }
  }

  resetEncoderValues();

  if(!moduleSucceeded){storedFaultCodes.enqueue(3);}
}

void doGyroDiag(){
  bool moduleSucceeded = false;
  
  resetGyroStartAndEnd();

  if(getGyroValueAtStart() == getGyroValueAtEnd()){
    int randomValue = random(0, 1000);
    
    setGyroValueAtStart(randomValue);
    setGyroValueAtEnd(randomValue);

    if(getGyroValueAtStart() == randomValue && getGyroValueAtEnd() == randomValue){
      if(getGyroZ() <= 180 && getGyroZ() >= -180){
        moduleSucceeded = true;
      }
    }
    
  }

  if(!moduleSucceeded){storedFaultCodes.enqueue(4);}
}

void doJoystickDiag(){
  bool moduleSucceeded = false;

  if(getQuadrantNumberFromAngle(10) == 1){
    if(getQuadrantNumberFromAngle(100) == 4){
      if(getQuadrantNumberFromAngle(190) == 3){
        if(getQuadrantNumberFromAngle(280) == 2){
          
          if(calculateLeftMotorSpeed(100, 0) == 255){
            if(calculateRightMotorSpeed(100, 0) == -255){
              moduleSucceeded = true;
            }
          }
          
        }
      }
    }
  }

  if(!moduleSucceeded){storedFaultCodes.enqueue(5);}
}

void doLocalizationDiag(){
  bool moduleSucceeded = false;



  //if(!moduleSucceeded){storedFaultCodes.enqueue(6);}
}

void doMotorcontrolDiag(){
  bool moduleSucceeded = false;



  //if(!moduleSucceeded){storedFaultCodes.enqueue(7);}
}

void doSensorsDiag(){
  bool moduleSucceeded = false;



  //if(!moduleSucceeded){storedFaultCodes.enqueue(8);}
}

void doSerialComDiag(){
  bool moduleSucceeded = false;



  //if(!moduleSucceeded){storedFaultCodes.enqueue(9);}
}
