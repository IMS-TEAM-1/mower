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




send_dict = {'FORWARD'      : '1',
             'BACKWARD'     : '3',
             'LEFT'         : '4',
             'RIGHT'        : '2',
             'STOP'         : '0',
             'NONE'         : '0',

             'POS'          : 'P',
             'MANUAL'       : 'M',
             'AUTONOMOUS'   : 'A',
             'CAPTURE'      : 'C',
             'DIAGNOSTIC'   : 'D',
             'STANDBY'      : 'S',
             'Hello'        : 'H',
             'ack'          : '!',
             'nok'          : '?'}

recv_dict = dict( (v,k) for (k,v) in send_dict.items() )


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
        greeting        = send_dict['Hello']
        self.send_serial(greeting)
        time.sleep(0.050) # let's wait before reading
        received        = self.receive_serial()
        expected_reply  = greeting + send_dict['ack']

        while received != expected_reply:
            print('arduinoHello(): not ready, '
                  + f'instead got <<<{received}>>>')
            self.send_serial(send_dict['Hello'])
            time.sleep(2)
            received = self.receive_serial()
        print('arduinoHello(): ready')


    def send_serial(self, message: str):
        """
        Send a message over the serial connection.

        Parameters:
            message (str): Message to be sent. Newlines are added to it.

        RNR4
        """
        print(f'ARDUINO <==== <<{message}>>')
        self.ser.write((message + '\r\n').encode('ascii'))


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

        ard_states = ['AUTONOMOUS', 'MANUAL', 'CAPTURE', 'STANDBY', 'DIAGNOSTIC']
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


                ##############################
                ## MANUAL COMMAND           ##
                ##############################
                elif message == 'MANUAL':

                    valid_manual_commands = ['FORWARD',
                                             'BACKWARD',
                                             'LEFT',
                                             'RIGHT',
                                             'STOP',
                                             'NONE']
                    ok_pl = payload in valid_manual_commands

                    if not ok_pl:
                        print(f'ARD: bad manual command <<<{payload}>>>')

                    if payload == 'STOP':
                        payload = 'NONE' # because of the arduino code

                    ard_manual_command = send_dict[payload] # get the number
                    self.send_serial(ard_manual_command)
                    from_ard = self.receive_serial()
                    expectation = ard_manual_command + send_dict['ack']
                    if from_ard != expectation:
                        print(f'arduino.py: Expected {expectation}')
                        print(f'arduino.py:      Got {from_ard}')
                    else:
                        ard_state = 'MANUAL'

                elif message == 'AUTONOMOUS':
                    print(f'ARD: got {message}')

                    self.send_serial(send_dict['AUTONOMOUS'])
                    from_ard = self.receive_serial()
                    expectation = send_dict['AUTONOMOUS'] + send_dict['ack']
                    if from_ard != expectation:
                        print(f'arduino.py: Expected {expectation}')
                        print(f'arduino.py:      Got {from_ard}')
                    else:
                        ard_state = 'AUTONOMOUS'

                elif message == 'STANDBY':
                    self.send_serial(send_dict['STANDBY'])

                    expectation = send_dict['STANDBY'] + send_dict['ack']
                    from_ard = self.receive_serial()
                    if from_ard != expectation:
                        print(f'arduino.py: Expected {expectation}')
                        print(f'arduino.py:      Got {from_ard}')
                    else:
                        ard_state = 'STANDBY'

                elif message == 'DIAGNOSTIC':
                    self.send_serial(send_dict['DIAGNOSTIC'])
                    ard_state = 'DIAGNOSTIC'
                    diagnostic_ok = send_dict['DIAGNOSTIC'] + send_dict['NONE']
                    do_loop = True
                    while do_loop:
                        if len(self.receive_serial()) > 0:
                            from_ard = self.receive_serial()
                            do_loop = False
                    if from_ard == diagnostic_ok:
                        print(f'Recieved Diagnostic OK!')
                    elif len(from_ard) > 0:
                        if from_ard[0] == send_dict['DIAGNOSTIC']:
                            failed_diag_modules = from_ard[1:]
                            print(f'Diagnostics NOT OK!')
                        else:
                            print(f'DIAG: recieved invalid char:    Got {from_ard}')

                # The camera just took a picture!
                # Let's go again!
                elif message == 'CAPTURE_DONE':
                    print(f'ARDUINO: got {message}, going again!')
                    self.send_serial(send_dict['CAPTURE'] + send_dict['ack'])

                elif message == 'SET_STATE':
                    if payload != 'MANUAL': # fugly hack, manual is sent elsewhere
                        self.send_serial(send_dict[payload])
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

                if len(ser_message) > 0:
                    charRecieved = ser_message[0]

                    # The arduino tells us it found an obstacle
                    if charRecieved == send_dict['CAPTURE']:
                        self.__to_controller('CAPTURE')
                        ard_state = 'CAPTURE'

                    # Deal with 'POS:123,78989'
                    elif charRecieved == send_dict['POS']:
                        coord_str   = ser_message[1:]
                        [x,y]       = coord_str.split(',')
                        self.__to_controller( 'POS', (int(x), int(y)) )

                    else:
                        # We did not catch this message
                        print('\tARD: unhandled serial_receive: '
                              + f'<<<{ser_message}>>>')


                else:
                    print('ARDUINO: Only recieved newline (empty message).')
            else:
                time.sleep(period)


        print('arduino loop done!')
