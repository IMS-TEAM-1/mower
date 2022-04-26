void setupMotors(){
  //Probably sets up PWM-control for motors
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

void doManualControlTick(){
  //moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentSpeedLeftMotor, currentAngle),calculateRightMotorSpeed(currentSpeedRightMotor, currentAngle));
  move(currentDirection, MOTOR_SPEED_MANUAL * PERCENTAGE_TO_PWM_FACTOR);
}

void isr_process_encoder1(void)
{
  if(digitalRead(Encoder_1.getPortB()) == 0){
    Encoder_1.pulsePosMinus();
  }else{
    Encoder_1.pulsePosPlus();
  }
}
void isr_process_encoder2(void)
{
  if(digitalRead(Encoder_2.getPortB()) == 0){
    Encoder_2.pulsePosMinus();
  }else{
    Encoder_2.pulsePosPlus();
  }
}

void move(direction_t direction, int speed)
{
  int leftSpeed = 0;
  int rightSpeed = 0;
  
  if(direction == FORWARD){
    activateManualForwardLEDs();
    leftSpeed = -speed;
    rightSpeed = speed;
  }else if(direction == BACKWARD){
    activateManualBackwardLEDs();
    leftSpeed = speed;
    rightSpeed = -speed;
  }else if(direction == LEFT){
    activateManualLeftLEDs();
    leftSpeed = -speed;
    rightSpeed = -speed;
  }else if(direction == RIGHT){
    activateManualRightLEDs();
    leftSpeed = speed;
    rightSpeed = speed;
  }else if(direction == NONE) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  
  Encoder_1.setTarPWM(leftSpeed);
  Encoder_2.setTarPWM(rightSpeed);

  _loop();

  currentDirection = NONE;
}

void moveBySeparateMotorSpeeds(int speedLeftMotor, int speedRightMotor){
  Encoder_1.setTarPWM(speedRightMotor);
  Encoder_2.setTarPWM(speedLeftMotor);
}

direction_t randomLeftOrRight(){
  int leftOrRight = random(3, 5);

  if (leftOrRight == 3){
    return LEFT;
  }else{
    return RIGHT;
  }
}

void stopMotors(){
  move(NONE, 0);
  currentSpeedLeftMotor = 0;
  currentSpeedRightMotor = 0;
  currentAngle = 0;
  currentDirection = NONE;
}
