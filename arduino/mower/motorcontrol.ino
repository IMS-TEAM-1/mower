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

void doRotationTest(){
  rotateByDegrees(90, LEFT, MAX_MOTOR_SPEED);

  _delay(3);

  rotateByDegrees(180, RIGHT, MAX_MOTOR_SPEED);

  _delay(3);

  rotateByDegrees(270, RIGHT, MAX_MOTOR_SPEED);

  _delay(3);

  rotateByDegrees(360, LEFT, MAX_MOTOR_SPEED);
}

void rotateByDegrees(int degreesToRotate, direction_t rotateLeftOrRight, int motorSpeed) {
  if(rotateLeftOrRight == LEFT){
    if(calculateFullCirclesNeeded(degreesToRotate) > 0){
      rotateFullCircles(calculateFullCirclesNeeded(degreesToRotate), rotateLeftOrRight, motorSpeed);
    }
    
    int calcDegreesToRotate = getGyroZ() - ((degreesToRotate - ROTATING_LEFT_MOMENTUM_OFFSET) % 360);

    if(calcDegreesToRotate >= -180){
      while(getGyroZ() > calcDegreesToRotate){
        move(LEFT, motorSpeed);
      }
    }
    else{
      int degreesDelta = abs(calcDegreesToRotate) % 180;
      int degreeToRotateTo = (180 - degreesDelta) * 1;
      
      bool wasInLowerValueHalf = false;
      bool wentOverHighestPeak = false;
      
      while(!wentOverHighestPeak){
        move(LEFT, motorSpeed);
        if(getGyroZ() >= -180 && getGyroZ() < 90){
          wasInLowerValueHalf = true;
        }
        if(wasInLowerValueHalf && getGyroZ() > 90){
          wentOverHighestPeak = true;
        }
      }
      while(getGyroZ() > degreeToRotateTo){
        move(LEFT, motorSpeed);
      }
    }
  }
  
  else if(rotateLeftOrRight == RIGHT){
    if(calculateFullCirclesNeeded(degreesToRotate) > 0){
      rotateFullCircles(calculateFullCirclesNeeded(degreesToRotate), rotateLeftOrRight, motorSpeed);
    }

    int calcDegreesToRotate = getGyroZ() + ((degreesToRotate - ROTATING_RIGHT_MOMENTUM_OFFSET) % 360);

    if(calcDegreesToRotate <= 180){
      while(getGyroZ() < calcDegreesToRotate){
        move(RIGHT, motorSpeed);
      }
    }
    else{
      int degreesDelta = abs(calcDegreesToRotate) % 180;
      int degreeToRotateTo = (180 - degreesDelta) * -1;
      
      bool wasInUpperValueHalf = false;
      bool wentOverHighestPeak = false;
      
      while(!wentOverHighestPeak){
        move(RIGHT, motorSpeed);
        if(getGyroZ() > 0 && getGyroZ() <= 180){
          wasInUpperValueHalf = true;
        }
        if(wasInUpperValueHalf && getGyroZ() < -90){
          wentOverHighestPeak = true;
        }
      }
      while(getGyroZ() < degreeToRotateTo){
        move(RIGHT, motorSpeed);
      }
    }
  }
  else{
    Serial.println("Recieved wrong parameter in: rotateByDegrees");
  }
  stopMotors();
}

void rotateFullCircles(int amountOfCircles, direction_t rotateLeftOrRight, int motorSpeed){
  for(int i = 0; i < amountOfCircles; i++){
    int angleNow = getGyroZ();
    
    bool beenInFirstQuadrant = false;
    bool beenInSecondQuadrant = false;
    bool beenInThirdQuadrant = false;
    bool beenInFourthQuadrant = false;
    
    bool circleRotated = false;

    //Rotate until first half of the circle is completed
    while(!circleRotated){
      if(getGyroZ() <= 0 && getGyroZ() > -90){
        beenInSecondQuadrant = true;
      }
      else if(getGyroZ() <= -90 && getGyroZ() > -180){
        beenInThirdQuadrant = true;
      }
      else if(getGyroZ() <= 180 && getGyroZ() > 90){
        beenInFourthQuadrant = true;
      }
      else if(getGyroZ() <= 90 && getGyroZ() > 0){
        beenInFirstQuadrant = true;
      }

      if(beenInFirstQuadrant && beenInSecondQuadrant && beenInThirdQuadrant && beenInFourthQuadrant){
        if(rotateLeftOrRight == LEFT){
          while(getGyroZ() > angleNow){
            //Serial.println(getGyroZ());
            move(rotateLeftOrRight, motorSpeed);
          }
          circleRotated = true;
        }
        else if(rotateLeftOrRight == RIGHT){
          while(getGyroZ() < angleNow){
            //Serial.println(getGyroZ());
            move(rotateLeftOrRight, motorSpeed);
          }
          circleRotated = true;
        }
      }

      move(rotateLeftOrRight, motorSpeed);
      //Serial.println(getGyroZ());
    }
  }
}

int calculateFullCirclesNeeded(int degreesToRotate){
  return abs(degreesToRotate)/360;
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
