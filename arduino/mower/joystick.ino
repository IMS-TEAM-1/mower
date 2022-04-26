/*
 * This file contains functions which can take input from a joystick and covert it to smooth driving.
 * However, this may not be used sinc the joystick probably will not get implemented.
 */

int currentSpeedLeftMotor = 0;
int currentSpeedRightMotor = 0;
int currentAngle = 0;

//Calculates what speed the motors should go in when getting length and angle from the PI, from the app, from the joystick
int calculateLeftMotorSpeed(double circleLength, double circleAngle){
  if(getQuadrantNumberFromAngle(circleAngle) == 1){
    return (1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 4){
    return (-1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 3){
    return ((-1*circleLength*((225-circleAngle)/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 2){
    return ((-1*circleLength*((1*(315-circleAngle))/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else{
    return 0;
  }
}

int calculateRightMotorSpeed(double circleLength, double circleAngle){
  if(getQuadrantNumberFromAngle(circleAngle) == 1){
    return ((-1*circleLength*((45-circleAngle)/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 4){
    return ((-1*circleLength*((135-circleAngle)/45)) * PERCENTAGE_TO_PWM_FACTOR);
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 3){
    return (1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else if(getQuadrantNumberFromAngle(circleAngle) == 2){
    return (-1*circleLength) * PERCENTAGE_TO_PWM_FACTOR;
  }
  else{
    return 0;
  }
}

//Used to get correct math
int getQuadrantNumberFromAngle(int circleAngle){
  if(circleAngle >= 0 && circleAngle <= 90){ return 1; }
  if(circleAngle > 90 && circleAngle <= 180){ return 4; }
  if(circleAngle > 180 && circleAngle < 270){ return 4; }
  if(circleAngle >= 270 && circleAngle <= 360){ return 3; }
  else{ return 0; }
}
