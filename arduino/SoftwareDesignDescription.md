
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

The file 'mower.ino' contains the main setup and loop of the robot.
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

The main loop of the mower is also rather simple to understand; it consits of a state machine, switching what function it should execute depending on the state.
Firstly, it checks whether it should update (through the serial communication)
