
# Mower Arduino documentation

The Arduino-software is built in various files called 'modules'.
Each module is responsible for its own section of the program and, to the most part, only contain functions related to the title of the module (exceptions with diagnostic code).
The modules that can be found are:

mower.ino (main program of the system)
autonomous.ino (code relatated to the mower moving in an autonomous way)
diagnostics.ino (self-diagnostics for higher requirement, checks the vital parts of the system)
encoder.ino (handling the pulses genereated by the encoders and how to retrieve the data)
gyro.ino (handles gyroscope data used during localization)
joystick.ino (not used, planned to be used in conjuction with a joystick in the app)
led.ino (LED functionality, found on the Arduino board)
localization.ino (all dead-reckoning funcionality, stores x- and y coordinate in centimeters)
motorcontrol.ino (controlling functionality of the motors)
sensors.ino (simple functions of the line follower sensor (IR-sensor) and collision sensor (ultrasonic sensor))
serial.ino (handler of serial communication with the Pi)
testing_code (code used when testing various functions when developing the code)


The following document will explain each module and the vital parts of that specific module.


## "mower.ino"

The file 'mower.ino' contains the main setup and main loop of the robot.
It is in this file all includes are found, the initial setup of sensors and similar are found and the main state machine.

The setup is rather simple to understand; is initializes the modules which needs some start value.

```{.ino}
void setup() {
  setupSerial();
  setupEncoderInterrupts();
  setupMotors();
  currentState = STANDBY;
  rgbled_0.setpin(44);
  gyro.begin();
  randomSeed(analogRead(0));
}
```

The main loop of the mower is also rather simple to understand; it consists of a state machine, switching what function it should execute depending on the state.
The loop checks whether it should update (through the serial communication). The 'true' here means that if there is any communication, try to read it and then acknowledge it back to the Pi.
Then, it makes sure that the correct state-function is being run by changing the switch case depending on the current state.

```{.ino}
void loop() {
  doSerialTick(true);
  
  switch(currentState){
    case(STANDBY):
      resetStateLEDs();
      stopMotors();
      break;
    case(AUTONOMOUS):
      doAutonomousTick();
      break;
    case(MANUAL):
      resetStateLEDs();
      doManualControlTick();
      break;
    case(DIAGNOSTIC):
      doSelfDiagnosticTest();
      break;
  }
}
```

## "autonomous.ino"

The self-driving/autonomous code can be found within this file. There is primaily one funnction that runs all of the autonomous driving but other functions are found witihin this main 'tick-function'.
The concept is rather simple: if we have not colided with anything or driven over the boundary, continously send our coordinates to the backend and drive forward.
If any of the sensors are triggered, there is a procedure to follow, found within the nested functions.

```{.ino}
void doAutonomousTick(){
  //If any of the sensors are triggered, stop, and do the appropiate procedure
  if(getUltraSonicSensorTriggered()){
    doAutonomousUltraSonicProcedure();
  }
  if(getLineFollowerTriggered()){
    doAutonomousLineFollowerProcedure();
  }

  doLocalizationTick();  //If we should send our location to the Pi, do that

  activateAutonomousForwardLEDs();
  move(FORWARD, MOTOR_SPEED_AUTONOMOUS_FORWARD * PERCENTAGE_TO_PWM_FACTOR); //Always move forward
}
```

## "config.h"

This file is self-explanatory, it contains a multitude of defines and other data types used within the software to avoid magic numbers and improve readability.

## "diagnostics.ino"

The 'diagnostics-'file contain all of the functionality used when the mower should self-diagnose.
It simply resets the module it should diagnose (being first the main code in the mower.ino file) and iterates through all of the modules.
If any errors are found within the modules, an id of that specific module is added to a queue which is read out after the diagnostics is complete and the mower can then determine if it failed the test or not.
Either way, the mower sends back either "DIAGNOSTIC:ok" if successful or for example "DIAGNOSTIC:1,3,8" if module 1, 3 and 8 failed.
Below you will see the start of the switch case but it follows the same structure for the following modules.

```{.ino}
void doSelfDiagnosticTest(){
  moduleCurrentlyDiagnosing = Diag_Main;
  while(!storedFaultCodes.isEmpty()){	//Clear all fault codes
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
    ...
  }
  ...
}
```

## "encoder.ino"

The 'encoders' are the so to speak 'counters' that the mower use which are located within the motors.
These encoders count pulses when the motors are turning, creating the possibility to calculate how much the motors have turned either forward or backwards.
Thanks to this, the encoder data can be used to fulfil the localization requirements, making it possible to calulate the 'local' coordinates based on how much the motors have rotated.

The code within this file is self-explanatory.

## "gyro.ino"

The encoders metioned generate the data needed to know how far the mower has gone but not in what direction. This is why the gyro is used within the mower.
The gyroscope's Z-axis data circles within -180 and 180, where 0 is straight ahead and -180/180 is when the mower is directed backwards. -90 is 90 degrees to the left and 90 is 90 degrees to the right.
The 'gyroAtStart' and 'gyraAtEnd' are parameters used to remember where the mower started from and where it ended, making it possible to calculate the average gyro value from one movement, since that lowers the error margin

Other than that, some simple set, get and reset functions are found within this file related to the gyro values.

## "joystick.ino"

This file contains logic which was planned to be implemented in the beginning but however got scrapped. The file remains though.
It contains somewhat confusing math due to how the joystick was planned to work but is functional to what was planned at first.

## "led.ino"

This file simply contains various void-functions which set the LEDs ontop of the Arduino board to specific RGB-color.
These LEDs are primarily used to indicate what main-state the mower is in but also other things such as in what direction the mower is moving and what module the self-diagnostics is currently testing.
However, when there are no exceptions, the 'main-reset-function' is called, setting the LEDs back to the initial state based on the current mower state.

```{.ino}
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
```

## "localization.ino"

All dead-reckoning functionality is found within this module.
The concept of how it works is that the encoders in the motors update the pulses stored in the program when the wheels are turning.
Based on what values the encoders give, is how the position is determined.
This method is quite optimized for this specific robot and has close to millimeter accuracy.
However, when used for a longer period of time, the position deviates.
The solution to this would either be complementary GPS/GNSS, better encoders or some other way of validating the rough position to calibrate.

The last position the mower has been at is stored making it possible to later transmit the location to the Pi to redirect it to the backend and app.
It does not always update the location though, only when a period of time has elapsed (currently one second).
If the period has elapsed is when the current location is calculated and then transmitted to the Pi.

```{.ino}
void doLocalizationTick(){
  if(millis() > timeForNextLocationTick){
    timeForNextLocationTick = timeForNextLocationTick + AUTONOMOUS_LOCATION_TICK_TIME_MS;

    calculateAndUpdateXAndYCoordinates();

    sendSerialCoordinates();
  }
}
```

How the location is calculated is found in the following function:

```{.ino}
void calculateAndUpdateXAndYCoordinates(){
  float newXCoordinate;
  float newYCoordinate;
  
  setGyroValueAtEnd(getGyroZ());
  
  float calcTemp = ((getAverageGyroValue() + LOCALIZATION_CIRCLE_ROTATION_OFFSET) * DEGREES_TO_RADIAN_FACTOR);

  newYCoordinate = getCoordinateY() + (getDistanceTravelled() * sin(calcTemp));
  newXCoordinate = getCoordinateX() + (getDistanceTravelled() * (cos(calcTemp) * -1)); //*-1 since we want the x-axis to be inverted due to the offset of 90 degrees

  setCoordinateX(newXCoordinate);
  setCoordinateY(newYCoordinate);

  resetEncoderValues();
}
```

Other functions are more of get, set and reset-type functions and the main calculation-function.


## "motorcontrol.ino"

All functions regarding the motors and how they are controlled are found here.
Basic operations as moving in a specific direction can be found but also more complex such as moving for a specific amount of millimeters, milliseconds or doing specific procedures.
Rotating functions can also be found here.
A single global variable is used when either getting, setting or resseting the direction in which the mower should go, reducing the risk of accidently moving is some direction by hard-coded functions.

The single tick-function found in this file is a simple move-function depending on what direction is recieved from the app via Bluetooth.
An example on how the joystick would be used is found as a comment.

```{.ino}
void doManualControlTick(){
  /*
   * If joystick is wanted:
   * moveBySeparateMotorSpeeds(calculateLeftMotorSpeed(currentJoysticSpeedLeftMotor, currentAngleJoystick),calculateRightMotorSpeed(currentJoysticSpeedRightMotor, currentAngleJoystick));
   */
   move(getCurrentDirection(), MOTOR_SPEED_MANUAL * PERCENTAGE_TO_PWM_FACTOR);
}
```

How the motors are controlled depening on the speed and direction is done by the following function:

```{.ino}
void move(direction_t direction, int speed)
{
  setCurrentDirection(direction);	//Update the stored direction
  float leftSpeed = 0;			//Create temp-variables
  float rightSpeed = 0;
  
  if(getCurrentDirection() == FORWARD){		//Depending on what direction the mower should move, the motors should move in opposite direction. Also, a deviation factor can be found, due to warped belts
    activateManualForwardLEDs();
    leftSpeed = -speed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = speed;
  }
  else if(getCurrentDirection() == BACKWARD){
    activateManualBackwardLEDs();
    leftSpeed = speed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -speed;
  }
  else if(getCurrentDirection() == LEFT){
    activateManualLeftLEDs();
    leftSpeed = -speed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = -speed;
  }
  else if(getCurrentDirection() == RIGHT){
    activateManualRightLEDs();
    leftSpeed = speed * MOTOR_DEVIATION_FACTOR;
    rightSpeed = speed;
  }
  else if(getCurrentDirection() == NONE) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  
  setEncoderPwm(1, leftSpeed);	//Here, the motor speed is actually set
  setEncoderPwm(2, rightSpeed);

  _loop();
}
```

## "sensors.ino"

This file contains two simple functions for both the IR-sensor and the ultrasonic sensor used for autonomous driving.

## "serial.ino"

This file handles all of the serial communication done between the Arduino and the Pi.
How our own protocol is defined is that firstly, the type of parameter is sent and sometimes some type of either acknowedlgeemnt or payload after a colon.
However, if the Arduino recieves some type of message it does not support, it sends back the message followed by a colon and "nok".

What the messages the mower can retrieve and understand are:

1. STANDBY (makes the mower stand still)
2. AUTONOMOUS (makes the mower change to be autonomous)

3. MANUAL:NONE (makes the mower change to manual state (manual control by Bluetooth) but without any direction (making it stand still))
4. MANUAL:FORWARD (change to manual (Bluetooth control), and move forward)
5. MANUAL:BACKWARD (change to manual (Bluetooth control), and move backward)
6. MANUAL:LEFT (change to manual (Bluetooth control), and move left)
7. MANUAL:RIGHT (change to manual (Bluetooth control), and move right)

8. DIAGNOSTIC (makes the mower perform a self-diagnostic)


Example when successful:

1. Pi: "AUTONOMOUS"
2. Arduino: "AUTONOMOUS:ack"

Example when not successful:

1. Pi: "AUTONOMOUSS"
2. Arduino: "AUTONOMOUSS:nok"


Another example of when moving in manual mode:

Example when successful:

1. Pi: "MANUAL:FORWARD"
2. Arduino: "FORWARD:ack"

Example when not successful:

1. Pi: "MANUAL:FORWARDD"
2. Arduino: "MANUAL:FORWARDD:nok" - Note: it says ":nok" on the entire message it recieved, which was "MANUAL:FORWARDD".


The main 'tick-function' handling the serial communication can be found within the file.
It is run continously within the main loop.
However, it should not overwhelm the backend, so it updates periodically (set now to 300ms intervals)
The function is defined as:

```{.ino}
void doSerialTick(bool ack){
  if(millis() - timeAtLastSerialUpdate > SERIAL_UPDATE_FREQUENCY_MS){	//Go in if we should update
    if(numberOfTicksMissed > MAX_ALLOWED_MISSED_SERIAL_TICKS){
      currentState = STANDBY;
      numberOfTicksMissed = 0; //Otherwise, the mower get locked. Can be removed if the mower has to be reset if any problem occurs with the communication
    }
    else{
      timeAtLastSerialUpdate = millis();
      
      readSerialData(ack);	//The ack follows within the functions until we either do or doesnt acknowledge the message.

      numberOfTicksMissed = 0;	//Should be removed when in final use, only here for when testing through Arduino monitor
    }
  }
  clearStoredMessages();	//When we have handled the messages, reset them to avoid that the mower repeats some actions
}
```

## "testing_code.ino"

This file only contain functions used during development which is worth saving.
These are not used in the main code.


## "#M1.1"

* "The Mower shall be capable of running autonomously within a confined area".

How the Arduino solves this is by utilizing the two IR/line-follower sensors mounted in the front of the robot.
These work by looking at a reflective surface and reads whether if the IR light sent can be read.
These are used, in this application, to detect black electrical tape which acts as the boundary of the confined area.
Thanks to this, the mower can detect the boundary, back up, rotate, and move in another direction.
This functionality can be found within the file "autonomous.ino".

The code is explained above, under the "autonomous.ino" heading.
The code for reading the sensors can be found within the file "sensors.ino"

If a line if found, this is the procedure the mower follows:

```{.ino}
//If line follower triggered, do this
void doAutonomousLineFollowerProcedure(){
  stopMotorsMS(1000);	//Make sure that the mower has stopped

  calculateAndUpdateXAndYCoordinates();	//Send the current position to the Pi

  doReverseProcedure(); //Back up, rotate, and continue moving autonomously
}
```

Reading the sensor:


```{.ino}
//Used to find black tape
bool getLineFollowerTriggered(){
  return (lineFollowerSensor.readSensors() == 0.000000); //Returns true if above black tape
}
```

## "#M1.2"

* "The Mower shall be able to avoid collision objects during autonomous operation."

Our solves this thanks to the ultrasonic sensor.
It sends out short bursts which can be detected and calulated based on the ToF and therefore calulate the distance between the sensor and the mower.
This enables the mower to know whehter something is infront of it and if it should avoid it by backing up, rotate, and move another direction.

The code for this can also be found within the file "autonomous.ino".
The code for reading the sensor can be found within the file "sensors.ino"

The code is rather similar to the code for the ultrasonic sensor, seen here:

```{.ino}
//If ultra sonic sensor triggered, do this
void doAutonomousUltraSonicProcedure(){
  stopMotorsMS(1000);	//Make sure that the mower has stopped

  calculateAndUpdateXAndYCoordinates();	//Send the current position to the Pi
  
  sendSerialUltraSonicTriggered();
  
  waitForImageCaptured();

  doReverseProcedure(); //Back up, rotate, and continue moving autonomously
}
```

The difference is that the Pi should now take a picture of the object infront of it and send it to the backend.
The Arudino sends "CAPTURE" to the Pi, and waits for a period of time until the Pi has captured the object (ten seconds now).
If, however, the Arduino recieves "CAPTURE:ack", the Arudino continues with the autonomous driving since the image has been captured.
Else, it continous after ten seconds to not make the mower hardlocked.

Code for the image capture waiting:

```{.ino}
/*
 * This function is called when the Arduino should wait for the Pi to take the picture through the camera.
 * The mower stand still and waits for an acknowledgement.
 * After a set time-out value, the mower continoues anyway.
 */
void waitForImageCaptured(){
  bool doLoop = true;
  long timeToCapture = millis() + CAMERA_CAPTURE_TIME;
  
  while(doLoop){
    stopMotorsMS(1000);
    doSerialTick(false);	//We only want to read if any message is on the bus, we do not want to acknowledge it since the message should be an acknowledgment to begin with
    if(recievedCaptureAck()){
      doLoop = false;
    }
    else if(millis() > timeToCapture) {
      Serial.println(getRecievedSerialDataFirstPart());
      Serial.println("ERROR IN CAMERA CAPTURE");
      doLoop = false;
    }
  }
}
```

## "#M1.3"

* "The Mower shall be able to accept and execute drive commands given by a remove device."

This is solved indirectly through the Pi.
Serial communication is held with the Pi directly, nothing else.
The Pi however, is connected to both the backend via Internet and an Android app via Bluetooth.
In both ways, the Pi can transmit and recieve data and transfer it to the Arduino.

Commands the mower can accept are mentioned under the "serial.ino" heading and "motorcontrol.ino".
These commands either make the mower stand still, move manually (via Bluetooth) or autonomously.
The non-drive-command is to do a self-diagnostic.

## "#M1.4"

* "The mower shall use a camera and send images to the backend REST API when collision avoidance occurs."

As mentioned under "#M.1.2", the Arduino sends "CAPTURE" to the Pi and waits for an image to be taken.
This is done when the ultrasonic sensor is triggered during autonomous driving.

The true implementation of fulfilling the requirement is found within the Pi SDD.

## "Process requirements"

# "Minimum"

* "The code shall be under version control." - Done in GitHub.
* "There shall be a Software Design Description document where each source code component is described." - You are reading it now!
* "A basic Work Breakdown Structure (WBS) shall be created for the intended scope of the project." - We have used Trello and time estimations there. The team leader will create a WBS for the entire project, so it cannot be found here.
* "An individual log book for each team member shall be kept documenting what has been accomplished and what time has been spent." - Missing from some members.
* "An analysis of the project shall be done (including process, technical aspects, collaboration and results) and documented in a Lessons Learned Document." - Not done.

# "Medium"

* "The Software Design Description shall contain low level requirements linked to the high level requirements in this document as needed in order to implement the functionality." - From Trello:

1. Retrieve line follower data. - 2 (Difficulty, abstract)
2. Be able to control the motors. - 2
3. Be able to install Raspbian/Raspberry Pi OS. - 1
4. Establish communicaiton between Raspberry Pi and Arduino. - 2
5. Capture a picture with Raspberry Pi. - 3
6. Bug testing for each sprint. - 3
7. Transition form defined loop code to communication based code. - 2
8. Manage Wi-Fi connections. - 3
9. Establish a communication between the Pi and backend over Internet. - 3
10. Hanlde encoder and gyroscope data. - 7, Edited: 2
11. Implement all API-requests and posts. - 3
12. Implement dead reckoning function. - 3, Edited: 5
13. Document code in Arduino. - 2
14. Thread solution for Pi code. - 8
15. Do this SDD. - 2
16. Set up and implement serial protocol. - 3, Edited 1: 5, Edited 2: 7
17. Implement Pi Bluetooth. - 2, Edited: 5
18. Document code in Pi. - 2
19. Do complete system test. - 8

* "Each Low level requirement or related softeware work package shall contain an estimate of how much work that is required for completion of the task." - Seen above.
* "The sum of all tasks and estimated efforts shall be compared with the time fram and resource availability of the project and planned accordingly." - Done when project is over.
* "Completion of tasks and used effort shall be compared with the plan and the estimates." - Done when project is over.

# "Maximum"

* "The Software Design Document shall contain an architectural overvire of the system and its sub components." - An overview of the Arduino/mower is given here, a complete overview will be done in another document.
* "The source code shall be traceable to respective low level requirement." - Vital parts explained in this document.
* "Test Cases shall be defined for all requirements and a test report produced" - Explained under the heading "diagnostic.ino". This is solved by self-diagnostics which either returns "DIAGNOSTIC:ok" or for example "DIAGNOSTIC:2,3,5" if module 2, 3, and 5 failed, making this the "report".