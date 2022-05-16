/*
 * This module contain various LED-functions
 * The LEDs are found ontop of the Arduino board.
 * The LEDs are used to indicate what state the mower is in (STANDBY, AUTONOMOUS, MANUAL or DIAGNOSTIC).
 * 
 */

void resetStateLEDs(){
  switch(currentState){
    case(STANDBY):
      activateStandbyLEDs();
      break;
    case(AUTONOMOUS):
      activateAutonomousLEDs();
      break;
    case(MANUAL):
      activateManualLEDs();
      break;
  }
}

void deactivateLEDs(){
  rgbled_0.setColor(0, 0, 0, 0);
  rgbled_0.show();
}

void activateAllLEDsRGB(int r, int g, int b){
  rgbled_0.setColor(0, r, g, b);
  rgbled_0.show();
}

void activateStandbyLEDs(){
  rgbled_0.setColor(0, 0, 0, 100);
  rgbled_0.show();
}

void activateManualLEDs(){
  rgbled_0.setColor(0, 100, 100, 0);
  rgbled_0.show();
}

void activateAutonomousForwardLEDs(){
  rgbled_0.setColor(8, 0, 100, 0);
  rgbled_0.setColor(9, 0, 100, 0);
  rgbled_0.setColor(10, 0, 100, 0);
  rgbled_0.setColor(5, 0, 100, 0);
  rgbled_0.setColor(6, 0, 100, 0);
  rgbled_0.setColor(7, 0, 100, 0);
  rgbled_0.setColor(11, 0, 100, 0);
  rgbled_0.setColor(12, 0, 100, 0);
  rgbled_0.setColor(1, 0, 100, 0);
  rgbled_0.show();
}

void activateAutonomousLEDs(){
  rgbled_0.setColor(0, 0, 100, 0);
  rgbled_0.show();
}

void activateManualForwardLEDs(){
  rgbled_0.setColor(2, 100, 0, 0);
  rgbled_0.setColor(3, 100, 0, 0);
  rgbled_0.setColor(4, 100, 0, 0);
  rgbled_0.show();
}

void activateManualBackwardLEDs(){
  rgbled_0.setColor(8, 100, 0, 0);
  rgbled_0.setColor(9, 100, 0, 0);
  rgbled_0.setColor(10, 100, 0, 0);
  rgbled_0.show();
}

void activateManualRightLEDs(){
  rgbled_0.setColor(5, 100, 0, 0);
  rgbled_0.setColor(6, 100, 0, 0);
  rgbled_0.setColor(7, 100, 0, 0);
  rgbled_0.show();
}

void activateManualLeftLEDs(){
  rgbled_0.setColor(11, 100, 0, 0);
  rgbled_0.setColor(12, 100, 0, 0);
  rgbled_0.setColor(1, 100, 0, 0);
  rgbled_0.show();
}



/*
 * 
 * DIAGNOSTIC CODE
 * 
 */


int deactivateLEDsTest(){
  int errorCounter = 0;

  if(!rgbled_0.setColor(0, 0, 0, 0)){errorCounter ++;}
  
  rgbled_0.show();

  return errorCounter;
}

int activateStandbyLEDsTest(){
  int errorCounter = 0;
  
  if(!rgbled_0.setColor(0, 0, 0, 100)){errorCounter ++;}
  
  rgbled_0.show();

  return errorCounter;
}

int activateAutonomousForwardLEDsTest(){
  int errorCounter = 0;
  
  if(!rgbled_0.setColor(8, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(9, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(10, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(5, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(6, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(7, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(11, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(12, 0, 100, 0)){errorCounter ++;}
  if(!rgbled_0.setColor(1, 0, 100, 0)){errorCounter ++;}
  
  rgbled_0.show();

  return errorCounter;
}

int activateDiagnosticLEDsTest(){
  int errorCounter = 0;
  
  if(!rgbled_0.setColor(0, 0, 100, 0)){errorCounter ++;}
  
  rgbled_0.show();

  return errorCounter;
}

int activateAutonomousLEDsTest(){
  int errorCounter = 0;

  if(!rgbled_0.setColor(0, 0, 100, 0)){errorCounter ++;}

  rgbled_0.show();

  return errorCounter;
}

bool resetStateLEDsTest(){
  bool errorDetected = false;
  switch(currentState){
    case(DIAGNOSTIC):
      if(!activateDiagnosticLEDsTest()){errorDetected = true;}
      break;
    default:
      errorDetected = true;
      break;
  }
  return errorDetected;
}

void inDiagModuleLED(int index){
  for(int i = 1; i <= index; i++){
    rgbled_0.setColor(i, 100, 0, 100);
  }
  rgbled_0.show();
}

void inAutonomousDiagModuleLED(){
  rgbled_0.setColor(1, 100, 0, 100);
  rgbled_0.setColor(5, 100, 0, 100);
  rgbled_0.show();
}

void doDiagnosticDoneLEDSprial(bool wasSuccessful){
  resetStateLEDsTest();
  rgbled_0.show();
  delay(300);
  
  for(int i = 1; i <= 12; i++){
    rgbled_0.setColor(i, 100, 0, 100);
    rgbled_0.show();
    delay(100);
  }

  resetStateLEDsTest();
  rgbled_0.show();
  delay(300);

  if(wasSuccessful){
    doDiagnosticSucceededFlash();
  }
  else{
    doDiagnosticFailedFlash();
  }
}

void doDiagnosticSucceededFlash(){
  rgbled_0.setColor(0, 0, 100, 0);
  rgbled_0.show();
  delay(600);

  deactivateLEDs();
  rgbled_0.show();
  delay(600);

  rgbled_0.setColor(0, 0, 100, 0);
  rgbled_0.show();
  delay(600);

  deactivateLEDs();
  rgbled_0.show();
  delay(600);

  rgbled_0.setColor(0, 0, 100, 0);
  rgbled_0.show();
  delay(1000);
}

void doDiagnosticFailedFlash(){
  rgbled_0.setColor(0, 100, 0, 0);
  rgbled_0.show();
  delay(600);

  deactivateLEDs();
  rgbled_0.show();
  delay(600);

  rgbled_0.setColor(0, 100, 0, 0);
  rgbled_0.show();
  delay(600);

  deactivateLEDs();
  rgbled_0.show();
  delay(600);

  rgbled_0.setColor(0, 100, 0, 0);
  rgbled_0.show();
  delay(1000);
}
