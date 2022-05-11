"""
Bluetooth. Look in old_code/btserver.py for previous
implementation.
"""

import time
# import os

from threading import Thread
from queue     import Queue

import bluetooth as bt



class BtServer(Thread):
    """
    The BtServer class.
    """
    def __init__(self, uuid, q_to_controller):
        """
        Set up the variables needed for the object. Shourd not
        run any code that might fail, such as hardware interactions.
        That should be done in hello().
        """
        Thread.__init__(self)
        self.orders          = Queue(maxsize = 10)
        self.q_to_controller = q_to_controller

        self.uuid            = uuid
        self.server_socket   = None     # late init
        self.server_port     = None     # late init
        self.client_socket   = None     # late init
        self.client_info     = None     # late init

    def __del__(self):
        pass

    def hello(self):
        """
        Initialize the to allow connection with the app.
        """
        self.server_socket = bt.BluetoothSocket(bt.RFCOMM)
        self.server_socket.bind(("", bt.PORT_ANY))
        self.server_socket.listen(1)
        self.server_port = self.server_socket.getsockname()[1]

        bt.advertise_service(
            self.server_socket,
            "panzermower",
            service_id      = self.uuid,
            service_classes = [ self.uuid, bt.SERIAL_PORT_CLASS ],
            profiles        = [ bt.SERIAL_PORT_PROFILE ] )

    def connect_to_app(self):
        """
        Establish connection to the app.
        """
        sock, info = self.server_socket.accept()
        self.client_socket  = sock
        self.client_info    = info
        print("connected")

    def recieve_message_from_app(self):
        """
        Returning messages from the app.
        """
        return self.client_socket.recv(1024)

    def close_connection(self):
        """
        Closes connection to the app.
        """
        self.client_socket.close()
        print("disconnected")
        self.server_socket.close()


    def order(self, message, payload = None):
        """
        Is called by main/controller.
        """
        self.orders.put( (message, payload) )

    def to_controller(self, message, payload = None):
        """
        Sends a message to main/controller.
        """
        self.q_to_controller.put( (self, message, payload) )


    def run(self):

        running = True
        period  = 0.1

        while running:

            try:
                data = self.recieve_message_from_app()
            except IOError:
                self.close_connection()

            bt_data_available   = not (len(data) == 0)
            q_elem_available    = not self.orders.empty()

            if bt_data_available:
                # var data is non-empty!
                #
                # Examples of incoming data:
                #   "FORWARD"
                #   "BACKWARD"
                #   ....?
                if data == 'FORWARD':
                    self.to_controller('FORWARD')


            elif q_elem_available:
                (message, payload) = self.orders.get()

                if message == 'HIT_OBSTACLE':
                    pass

                if message == 'RESTART_SERVER':
                    pass

                if message == 'SOME_BT_ORDER_FROM_MAIN':
                    pass

                if message == 'EXIT':
                    running = False
                    print('btserver quits!')

            else:
                time.sleep(period)


        print('btserver loop done!')






