/*
 * Here, some utility functions can be found.
 */

void _delay(float seconds) {
  if(seconds < 0.0){
    seconds = 0.0;
  }
  long endTime = millis() + seconds * 1000;
  while(millis() < endTime) _loop();
}

void _loop() {
  Encoder_1.loop();
  Encoder_2.loop();
}

void moveForAmountOfTime(int ms){
  int endTime = millis() + ms;
  while(millis() < endTime) _loop();
}

void resetStateLEDs(){
  switch(currentState){
    case(STANDBY):
      activateStandbyLEDs();
      break;
    case(AUTONOMOUS):
      activateAutonomousLEDs();
      break;
    case(MANUAL):
      deactivateLEDs();
      break;
  }
}

void deactivateLEDs(){
  rgbled_0.setColor(0, 0, 0, 0);
  rgbled_0.show();
}

void activateStandbyLEDs(){
  rgbled_0.setColor(0, 0, 0, 100);
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
