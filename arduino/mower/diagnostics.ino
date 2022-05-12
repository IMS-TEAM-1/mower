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
        moduleCurrentlyDiagnosing = Diag_Encoder;
        break;
        
      case(Diag_Encoder):
        inDiagModuleLED(2);
        doEncoderDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Gyro;
        break;
        
      case(Diag_Gyro):
        inDiagModuleLED(3);
        doGyroDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Localization;
        break;

      case(Diag_Localization):
        inDiagModuleLED(4);
        doLocalizationDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Joystick;
        break;
        
      case(Diag_Joystick):
        inDiagModuleLED(5);
        doJoystickDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Motorcontrol;
        break;

        
      case(Diag_Motorcontrol):
        inDiagModuleLED(6);
        doMotorcontrolDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Sensors;
        break;
        
      case(Diag_Sensors):
        inDiagModuleLED(7);
        doSensorsDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_SerialCom;
        break;
        
      case(Diag_SerialCom):
        inDiagModuleLED(8);
        doSerialComDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Autonomous;
        break;
        
      case(Diag_Autonomous):
        inDiagModuleLED(9);
        doAutonomousDiag();
        delay(1000);
        moduleCurrentlyDiagnosing = Diag_Done;
        break;
        
       case(Diag_Done):
        if(storedFaultCodes.isEmpty()){
          sendDiagnosticSuccess();
          doDiagnosticDoneLEDSprial(true);
        }
        else{
          sendDiagnosticFailed(storedFaultCodes);
          doDiagnosticDoneLEDSprial(false);
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
  
  if(!moduleSucceeded){storedFaultCodes.enqueue(2);}
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

  if(!moduleSucceeded){storedFaultCodes.enqueue(3);}
}

void doLocalizationDiag(){
  bool moduleSucceeded = false;

  resetCoordinates();

  if(getCoordinateX() == 0 && getCoordinateY() == 0){
    int randomValue = random(0, 1000);

    setCoordinateX(randomValue);
    setCoordinateY(randomValue);

    if(getCoordinateX() == randomValue && getCoordinateY() == randomValue){
      moduleSucceeded = true;
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

void doMotorcontrolDiag(){
  bool moduleSucceeded = false;

  stopMotorsMS(1000);

  activateAllLEDsRGB(100, 0, 100);

  if(getCurrentJoysticSpeedLeftMotor() == 0 && getCurrentJoysticSpeedRightMotor() == 0 && getCurrentAngleJoystick() == 0 && getCurrentDirection() == NONE){
    if(moveTest(FORWARD, 100) == false){
      if(moveTest(BACKWARD, 50) == false){
        if(moveTest(LEFT, 150) == false){
          if(moveTest(RIGHT, 200) == false){
            if(moveTest(NONE, 0) == false){
              resetEncoderValues();
              moduleSucceeded = true;
            }
          }
        }
      }
    }
  }

  if(!moduleSucceeded){storedFaultCodes.enqueue(6);}
}

void doSensorsDiag(){
  bool moduleSucceeded = false;

  if(doLinefollowerSensorDiag()){
    Serial.println("1");
    if(doUltrasonicSensorDiag()){
      Serial.println("2");
      moduleSucceeded = true;
    }
  }

  if(!moduleSucceeded){storedFaultCodes.enqueue(7);}
}

bool doUltrasonicSensorDiag(){
  bool isFunctional = false;
  
  if(ultraSonicSensor.distanceCm() >= 0 && ultraSonicSensor.distanceCm() < ULTRA_SONIC_SENSOR_MAX_VALUE){
    isFunctional = true;
  }
  
  else{  //Is 400 when cable is unplugged but 400 can be read when plugged in as well
    bool doLoop = true;
    long timeToStopTest = millis() + TIME_TO_TEST_ULTRA_SONIC_SENSOR_MS;

    while(doLoop){
      if(ultraSonicSensor.distanceCm() >= 0 && ultraSonicSensor.distanceCm() < ULTRA_SONIC_SENSOR_MAX_VALUE){
        isFunctional = true;
        doLoop = false;
      }
      if(millis() >= timeToStopTest){
        doLoop = false;
      }
      rotateByDegrees(50, RIGHT, MAX_MOTOR_SPEED);
    }
    
  }

  stopMotorsMS(1000);

  return isFunctional;
}

//If the mower is above sensor when testing, move it and check again. If always triggered, probably something wrong with cable or sensor
bool doLinefollowerSensorDiag(){
  bool isFunctional = false;
  
  if(getLineFollowerTriggered()){
    rotateByDegrees(30, LEFT, HALF_MOTOR_SPEED);
    stopMotorsMS(1000);
    
    if(getLineFollowerTriggered()){
      rotateByDegrees(30, LEFT, HALF_MOTOR_SPEED);
      stopMotorsMS(1000);
      
      if(getLineFollowerTriggered()){
        rotateByDegrees(30, LEFT, HALF_MOTOR_SPEED);
        stopMotorsMS(1000);
        
        if(!getLineFollowerTriggered()){
          isFunctional = true;
        }
      }
      else{
        isFunctional = true;
      }
    }
    else{
      isFunctional = true;
    }
  }
  else{
    isFunctional = true;
  }

  return isFunctional;
}

void doSerialComDiag(){
  bool moduleSucceeded = false;

  clearStoredMessages();

  if(getRecievedSerialDataFirstPart() == "" && getRecievedSerialDataSecondPart() == ""){
    String s_temp = "DIAGNOSTIC_SERIAL:TEST";
    checkAndSetRecievedMessage(s_temp);

    if(getRecievedSerialDataFirstPart() == "DIAGNOSTIC_SERIAL" && getRecievedSerialDataSecondPart() == "TEST"){
      moduleSucceeded = true;
    }
  }

  if(!moduleSucceeded){storedFaultCodes.enqueue(8);}
}

void doAutonomousDiag(){
  bool moduleSucceeded = false;

  moduleSucceeded = autonomousDiagnosticTestSuccess();
  
  if(!moduleSucceeded){storedFaultCodes.enqueue(9);}
}
