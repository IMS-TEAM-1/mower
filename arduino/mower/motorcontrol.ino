int currentSpeedLeftMotor = 0;
int currentSpeedRightMotor = 0;
int currentAngle = 0;

void setupMotors(){
  //Probably sets up PWM-control for motors
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
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
    leftSpeed = -speed;
    rightSpeed = speed;
  }else if(direction == BACKWARD){
    leftSpeed = speed;
    rightSpeed = -speed;
  }else if(direction == LEFT){
    leftSpeed = -speed;
    rightSpeed = -speed;
  }else if(direction == RIGHT){
    leftSpeed = speed;
    rightSpeed = speed;
  }else if(direction = NONE) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  
  Encoder_1.setTarPWM(leftSpeed);
  Encoder_2.setTarPWM(rightSpeed);
}

void moveBySeparateMotorSpeeds(int speedLeftMotor, int speedRightMotor){
  //Serial.println(speedLeftMotor);
  //Serial.println(speedRightMotor);
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

//Calculates what speed the motors should go in when getting length and angle from the PI, from the app, from the joystick
int calculateLeftMotorSpeed(double circleLength, double circleAngle){
  //Serial.println("LEFT MOTOR SPEED: ");
  if(getQuadrantNumberFromAngle(circleAngle) == 1){
    //Serial.println(-1*circleLength);
    return (1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 4){
    //Serial.println(-1*circleLength*((135-circleAngle)/45));
    return (-1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 3){
    //Serial.println(1*circleLength);
    return ((-1*circleLength*((225-circleAngle)/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 2){
    //Serial.println(-1*circleLength*((-1*(315-circleAngle))/45));
    return ((-1*circleLength*((1*(315-circleAngle))/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else{
    return 0;
  }
}

int calculateRightMotorSpeed(double circleLength, double circleAngle){
  //Serial.println("RIGHT MOTOR SPEED: ");
  if(getQuadrantNumberFromAngle(circleAngle) == 1){
    //Serial.println(1*circleLength*((45-circleAngle)/45));
    return ((-1*circleLength*((45-circleAngle)/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 4){
    //Serial.println(-1*circleLength);
    return ((-1*circleLength*((135-circleAngle)/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 3){
    //Serial.println(-1*circleLength*((225-circleAngle)/45));
    return (1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 2){
    //Serial.println(1*circleLength);
    return (-1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else{
    return 0;
  }
}

int getQuadrantNumberFromAngle(int circleAngle){
  if(circleAngle >= 0 && circleAngle <= 90){ return 1; }
  if(circleAngle > 90 && circleAngle <= 180){ return 4; }
  if(circleAngle > 180 && circleAngle < 270){ return 4; }
  if(circleAngle >= 270 && circleAngle <= 360){ return 3; }
  else{ return 0; }
}

void resetMotorValues(){
  currentSpeedLeftMotor = 0;
  currentSpeedRightMotor = 0;
  currentAngle = 0;
}
