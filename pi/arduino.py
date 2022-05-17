"""
Module for communicating with the Arduino motor control and sensor hub.
Communication is done over serial connection (using pyserial).
Instructions are received through a queue that is an argument to the
constructor. The main loop being run (should be as a thread) is the
function run().
"""


from threading import Thread
from queue     import Queue

import time
import serial
# import select

class Arduino(Thread):
    """
    Class for managing communication with the Arduino over serial connection.
    Runs as a thread, invoked by a controller.
    """

    def __init__(self, device, baud, timeout, q_to_controller):
        """
        Constructor for the class managing communication with the Arduino.
        Details for the parameters are found in the pyserial library.
        """
        Thread.__init__(self)
        self.ser                = serial.Serial(device,
                                                baud,
                                                timeout = timeout)
        self.ser.reset_input_buffer()

        self.__orders           = Queue(maxsize = 10)
        self.q_to_controller    = q_to_controller

    def __del__(self):
        """
        Close the connection to the serial device.
        """
        self.ser.close()



    def hello(self):
        """
        Open up with a handshake to the robot.
        """
        greeting        =  'Hello'
        self.send_serial(greeting)
        time.sleep(0.050) # let's wait before reading
        received        = self.receive_serial()
        expected_reply  = f'{greeting}:ack'

        while received != expected_reply:
            print('arduinoHello(): not ready, '
                  + f'instead got <<<{received}>>>')
            self.send_serial('Hello')
            time.sleep(2)
            received = self.receive_serial()
        print('arduinoHello(): ready')


    def send_serial(self, message):
        """
        Send a message over the serial connection.

        Parameters:
            message (str): Message to be sent. Newlines are added to it.
        """
        self.ser.write((message + '\r\n').encode('ascii'))


    def receive_serial(self) -> str:
        """
        Receive a message from the serial connection. Reads until
        newline character(s) are found.

        Returns:
            A string with newline characters stripped.
        """
        line = self.ser.readline()
        line = str(line.decode('utf-8').strip())
        self.ser.reset_input_buffer()
        return line


    def order(self, message, payload = None):
        """
        Put a message into the queue provided with the
        constructor. This message will then be read in the
        main loop of the run() function.

        Argument message should be a tuple with two elements:
            First an instruction of some kind. The second
            argument is a placeholder for a payload.
            If there is no payload, the second argument
            should be None.

        Examples:
            ard = Arduino(...)
            ard.hello()
            ...
            ard.order( ('AUTONOMOUS', None) )
            ...
            ard.order( ('MANUAL', 'FORWARD') )

        Parameters:
            message (str) : The message to send to controller.
            payload : Optional argument. Could be of any type.
        """
        self.__orders.put( (message, payload) )



    def __to_controller(self, message , payload = None):
        """
        Complement to order(), to be used from within this module.
        Listener is other end of class variable q_to_controller
        """
        self.q_to_controller.put( (self, message, payload) )


    def run(self):
        """
        This is the main loop of the arduino. Because it's reading from
        two sources (orders and serial connection) it must periodically
        wake up and check both.
        """

        period = 0.050 # 50 ms == 20 Hz
        running = True

        ard_states = ['AUTONOMOUS', 'MANUAL', 'CAPTURE', 'STANDBY']
        ard_state = 'AUTONOMOUS'

        while running:

            # Documentation at
            #    https://pythonhosted.org/pyserial/pyserial_api.html
            # says:
            #       in_waiting
            #       Getter: Get the number of bytes 
            #               in the input buffer.
            #       Type:   int
            #       Return the number of bytes in the receive buffer.
            serial_available    = self.ser.in_waiting > 0
            q_elem_available    = not self.__orders.empty()



            ########################################
            ##      MESSAGE RECEIVED FROM MAIN    ##
            ########################################
            if q_elem_available:
                (message, payload) = self.__orders.get()

                if message == 'EXIT':
                    running = False
                    print('arduino quits!')

                elif message == 'MANUAL':
                    self.send_serial('MANUAL:NONE')

                    from_ard = self.receive_serial()
                    expectation = 'NONE:ack'
                    if from_ard != expectation:
                        print(f'arduino.py: Expected {expectation}')
                        print(f'arduino.py:      Got {from_ard}')
                    else:
                        ard_state = 'MANUAL'

                elif message == 'AUTONOMOUS':
                    print(f'ARD: got {message}')

                    self.send_serial('AUTONOMOUS')
                    from_ard = self.receive_serial()
                    expectation = 'AUTONOMOUS:ack'
                    if from_ard != expectation:
                        print(f'arduino.py: Expected {expectation}')
                        print(f'arduino.py:      Got {from_ard}')
                    else:
                        ard_state = 'AUTONOMOUS'

                elif message == 'STANDBY':
                    self.send_serial('STANDBY')

                    expectation = 'STANDBY:ack'
                    from_ard = self.receive_serial()
                    if from_ard != expectation:
                        print(f'arduino.py: Expected {expectation}')
                        print(f'arduino.py:      Got {from_ard}')
                    else:
                        ard_state = 'STANDBY'

                # The camera just took a picture!
                # Let's go again!
                elif message == 'CAPTURE_DONE':
                    print(f'ARDUINO: got {message}, going again!')
                    self.send_serial('CAPTURE:ack')

                elif message == 'SET_STATE':
                    self.send_serial(payload)
                    ard_state = payload

                else:
                    print('ARDUINO: Unhandled message '
                          + f'({message},{payload})')
                    # ard_state = 'STANDBY'



            ########################################
            ##      MESSAGE RECEIVED OVER SERIAL  ##
            ########################################
            elif serial_available:

                ## first check without colon
                ser_message = self.receive_serial()

                # The arduino tells us it found an obstacle
                if ser_message == 'CAPTURE':
                    self.__to_controller('CAPTURE')
                    ard_state = 'CAPTURE'

                # Deal with 'POS:123,78989'
                elif ser_message[0 : 3] == 'POS':
                    [_, coord_str]  = ser_message.split(':')
                    [x,y]           = coord_str.split(',')
                    self.__to_controller( 'POS', (int(x), int(y)) )

                else:
                    # We did not catch this message
                    print('\tARD: unhandled serial_receive: '
                          + f'<<<{ser_message}>>>')

            else:
                time.sleep(period)


        print('arduino loop done!')
