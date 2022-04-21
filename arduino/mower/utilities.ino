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

void deactivateLEDs(){
  rgbled_0.setColor(0, 0, 0, 0);
  rgbled_0.show();
}

void activateStandbyLEDs(){
  rgbled_0.setColor(0, 0, 0, 255);
  rgbled_0.show();
}

void activateAutonomousLEDs(){
  rgbled_0.setColor(0, 0, 255, 0);
  rgbled_0.show();
}

void activateManualLEDs(){
  rgbled_0.setColor(0, 255, 0, 0);
  rgbled_0.show();
}
