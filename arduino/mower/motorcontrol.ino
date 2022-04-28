//Probably sets up PWM-control for motors
void setupMotors(){
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
}

void doManualControlTick(){
  /*
   * If joystick is wanted:
   * moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentSpeedLeftMotor, currentAngle),calculateRightMotorSpeed(currentSpeedRightMotor, currentAngle));
   */
   move(currentDirection, MOTOR_SPEED_MANUAL * PERCENTAGE_TO_PWM_FACTOR);
}

/*
 * This is the main mowing function
 * This takes one input and one speed and the mower moves accordingly.
 */
void move(direction_t direction, int speed)
{
  float leftSpeed = 0;
  float rightSpeed = 0;
  
  if(direction == FORWARD){
    activateManualForwardLEDs();
    leftSpeed = -speed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = speed;
  }else if(direction == BACKWARD){
    activateManualBackwardLEDs();
    leftSpeed = speed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -speed;
  }else if(direction == LEFT){
    activateManualLeftLEDs();
    leftSpeed = -speed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -speed;
  }else if(direction == RIGHT){
    activateManualRightLEDs();
    leftSpeed = speed * MOTOR_DEVIATION_FACTOR;
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

//Used when both motors should move with various speeds, used in conjuction with joystick steering
void moveBySeparateMotorSpeeds(int speedLeftMotor, int speedRightMotor){
  Encoder_1.setTarPWM(speedRightMotor);
  Encoder_2.setTarPWM(speedLeftMotor);
}

//Used for turning either right or left when in autonomous mode
direction_t randomLeftOrRight(){
  int leftOrRight = random(3, 5);

  if (leftOrRight == 3){
    return LEFT;
  }else{
    return RIGHT;
  }
}

//Simply make the mower stop OBS: may have to used continously to actually make the mower stop
void stopMotors(){
  resetStateLEDs();
  move(NONE, 0);
  currentSpeedLeftMotor = 0;
  currentSpeedRightMotor = 0;
  currentAngle = 0;
  currentDirection = NONE;
}
