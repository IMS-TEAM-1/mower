
# Software Design Description Overview

The Software Design Description for this part of the project (Arduino & Raspberry Pi) is divided into three parts.

  * `./SoftwareDesignDescription.md`
  * `./arduino/SoftwareDesignDescription.md`
  * `./pi/SoftwareDesignDescription.md`

This document details the high level requirements and the process requirements.
The process requirements led us to low lever requirements, (called `RNRx`) which are listed below.


## Low Level Requirement Example

This example is derived as a low level requirement. Its description text is taken from our Trello board, which is how the work was organized.

> RNR4. Establish communication between Raspberry Pi and Arduino. - 2

When reading the code in `pi/arduino.py`, notice the last line in the function documentation comment.

```{.python}
    def receive_serial(self) -> str:
        """
        Receive a message from the serial connection. Reads until
        newline character(s) are found.

        Returns:
            A string with newline characters stripped.
        
        RNR4
        """
        line = self.ser.readline()
        line = str(line.decode('utf-8').strip())
        self.ser.reset_input_buffer()

        print(f'ARDUINO ====> <<{line}>>')
        return line
```

And in `arduino/mower/serial.ino` there's this comment.

```{.cpp}
 /*
  * Code here is due to RNR4 and RNR16
  */
```






# High Level Requirements


## `#M1.1`

* "The Mower shall be capable of running autonomously within a confined area".

How the Arduino solves this is by utilizing the two IR/line-follower sensors mounted in the front of the robot.
These work by looking at a reflective surface and reads whether if the IR light sent can be read.
These are used, in this application, to detect black electrical tape which acts as the boundary of the confined area.
Thanks to this, the mower can detect the boundary, back up, rotate, and move in another direction.
This functionality can be found within the file `autonomous.ino`.

The code is explained above, under the `autonomous.ino` heading.
The code for reading the sensors can be found within the file `sensors.ino`

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





## `#M1.2`

* "The Mower shall be able to avoid collision objects during autonomous operation."

Our solves this thanks to the ultrasonic sensor.
It sends out short bursts which can be detected and calculated based on the ToF and therefore calculate the distance between the sensor and the mower.
This enables the mower to know whether something is in front of it and if it should avoid it by backing up, rotate, and move another direction.

The code for this can also be found within the file `autonomous.ino`.
The code for reading the sensor can be found within the file `sensors.ino`.

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

The difference is that the Pi should now take a picture of the object in front of it and send it to the backend.
The Arudino sends `CAPTURE` to the Pi, and waits for a period of time until the Pi has captured the object (ten seconds now).
If, however, the Arduino receives "CAPTURE:ack", the Arudino continues with the autonomous driving since the image has been captured.
Else, it continues after ten seconds to not make the mower hard locked.

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



## `#M1.3`

* "The Mower shall be able to accept and execute drive commands given by a remove device."

This is solved indirectly through the Pi.
Serial communication is held with the Pi directly, nothing else.
The Pi however, is connected to both the backend via Internet and an Android app via Bluetooth.
In both ways, the Pi can transmit and receive data and transfer it to the Arduino.

Commands the mower can accept are mentioned under the `serial.ino` heading and `motorcontrol.ino`.
These commands either make the mower stand still, move manually (via Bluetooth) or autonomously.
The non-drive-command is to do a self-diagnostic.



## `#M1.4`

* "The mower shall use a camera and send images to the backend REST API when collision avoidance occurs."

As mentioned under `#M.1.2`, the Arduino sends `CAPTURE` to the Pi and waits for an image to be taken.
This is done when the ultrasonic sensor is triggered during autonomous driving.

The true implementation of fulfilling the requirement is found within the Pi SDD.




# Process requirements



## Minimum

> "The code shall be under version control." - Done in GitHub.

> "There shall be a Software Design Description document where each source code component is described." - You are reading it now!

> "A basic Work Breakdown Structure (WBS) shall be created for the intended scope of the project." - We have used Trello and time estimations there. The team leader will create a WBS for the entire project, so it cannot be found here.

> "An individual log book for each team member shall be kept documenting what has been accomplished and what time has been spent." - Missing from some members.

> "An analysis of the project shall be done (including process, technical aspects, collaboration and results) and documented in a Lessons Learned Document." - Not done.




## Medium


> "The Software Design Description shall contain low level requirements linked to the high level requirements in this document as needed in order to implement the functionality."


These points are from our Trello board.
RNR is abbreviation of "Requirement NumbeR".
The numbers on each line is our difficulty estimation of the task.

  * RNR1. Retrieve line follower and ultrasonic data. - 2
  * RNR2. Be able to control the motors. - 2
  * RNR3. Be able to install Raspbian/Raspberry Pi OS. - 1
  * RNR4. Establish communication between Raspberry Pi and Arduino. - 2
  * RNR5. Capture a picture with Raspberry Pi. - 3
  * RNR6. Bug testing for each sprint. - 3
  * RNR7. Transition form defined loop code to communication based code. - 2
  * RNR8. Manage Wi-Fi connections. - 3
  * RNR9. Establish a communication between the Pi and backend over Internet. - 3
  * RNR10. Handle encoder and gyroscope data. - 7, Edited: 2
  * RNR11. Implement all API-requests and posts. - 3
  * RNR12. Implement dead reckoning function. - 3, Edited: 5
  * RNR13. Document code in Arduino. - 2
  * RNR14. Thread solution for Pi code. - 8
  * RNR15. Do this SDD. - 2
  * RNR16. Set up and implement serial protocol. - 3, Edit 1: 5, Edit 2: 7
  * RNR17. Implement Pi Bluetooth. - 2, Edited: 5
  * RNR18. Document code in Pi. - 2
  * RNR19. Do complete system test. - 8

>  "Each Low level requirement or related software work package shall contain an estimate of how much work that is required for completion of the task." - Seen above.

> "The sum of all tasks and estimated efforts shall be compared with the time frame and resource availability of the project and planned accordingly." - Done when project is over.

> "Completion of tasks and used effort shall be compared with the plan and the estimates." - Done when project is over.



# Maximum

> "The Software Design Document shall contain an architectural overvire of the system and its sub components." - An overview of the Arduino/mower is given here, a complete overview will be done in another document.

> "The source code shall be traceable to respective low level requirement." - Vital parts explained in this document.

> "Test Cases shall be defined for all requirements and a test report produced" - Explained under the heading `diagnostic.ino`. This is solved by self-diagnostics which either returns `DIAGNOSTIC:ok` or for example `DIAGNOSTIC:2,3,5` if module 2, 3, and 5 failed, making this the "report".

