"""
Module for communicating with the Arduino
motor control and sensor hub.
"""

from   time   import sleep
import serial

class Arduino:
    """
    Just a class.
    """
    def __init__(self, device, baud, timeout):
        self.ser = serial.Serial(device, baud, timeout = timeout)
        self.ser.reset_input_buffer()


    def __del__(self):
        pass


    def hello(self):
        """
        Open up with a handshake to the robot.
        """
        self.send_message('Hello')

        msg = self.receive_message()
        got_ack = msg == 'Hello:ack'

        while not got_ack:
            print('arduinoHello(): not ready, instead got "' + msg + '"')
            self.send_message('Hello')
            sleep(2)
            msg = self.receive_message()
            got_ack = msg == 'Hello:ack'
        print("arduinoHello(): ready")


    def ready(self):
        """
        Ready for what, precisely?
        """
        self.send_message('rdy')
        while self.receive_message() != 'rdy:ack':
            print("ready(): not ready")
            self.send_message('rdy')
            sleep(2)
        print("arduinoRdy(): ready")


    def post(self):
        """
        Imma put you in muh mailbox.
        """
        #print("arduinoPost(): not implemented")
        keyboard = input("write your command: ")
        self.send_message(keyboard)
        sleep(0.1)
        print(self.receive_message())


    def send_message(self, msg):
        """
        Send a message over the serial connection.
        """
        self.ser.write((msg + "\r\n").encode('ascii'))


    def receive_message(self):
        """
        Receive a message from the serial connection.
        """
        line = self.ser.readline()
        line = str(line.decode('utf-8').strip())
        return line
