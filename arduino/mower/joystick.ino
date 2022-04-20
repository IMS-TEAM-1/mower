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
