"""
Bluetooth. Look in old_code/btserver.py for previous
implementation.
"""

import time
# import os
import select

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
        Advertises the BT device as connectable server.
        """
        self.server_socket = bt.BluetoothSocket(bt.RFCOMM)
        self.server_socket.bind((self.uuid, bt.PORT_ANY))
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
        Creates socket connection to app.
        """
        self.client_socket, self.client_info = self.server_socket.accept()
        print(f'BTSERVER: ({self.client_socket},{self.client_info}) connected')
        return True

    def recieve_message_from_app(self):
        """
        Returns message from app.
        """
        return self.client_socket.recv(1024)

    def close_connection(self):
        """
        Closes socket connection and creates a new socket.
        """
        self.client_socket.close()
        self.server_socket.close()
        print('BTSERVER: close_connection()')


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

    def has_input(self, handle):
        return select.select([handle, ],[], [], 0.0)[0]

    def run(self):

        running = True
        period  = 0.1

        states = ['NO_CONN', 'MK_CONN', 'BT_CONN']
        state = 'NO_CONN'

        while running:

            q_elem_available = not self.orders.empty()

            run_bt = self.has_input(self.client_socket) if state == 'BT_CONN' else \
                     self.has_input(self.server_socket) if state == 'MK_CONN' else \
                     False # 'NO_CONN'

            if q_elem_available:
                (message, payload) = self.orders.get()

                if message == 'BACKEND_SAYS_ACTIVATE_BT':
                    print(f'BT got message <<<{message}>>>')
                    state = 'MK_CONN'

                elif message == 'BACKEND_SAYS_DEACTIVATE_BT':
                    print(f'BT got message <<<{message}>>>')
                    self.close_connection()
                    state = 'NO_CONN'
                    print('BT DEACTIVATED!')

                elif message == 'EXIT':
                    running = False
                    print('btserver quits!')
                else:
                    print(f'BT unrecognized order <<<{message}>>>')


            elif run_bt:
                if state == 'BT_CONN':
                    try:
                        bt_data = self.recieve_message_from_app() #Is blocking
                        bt_data = bt_data.decode('UTF-8').strip()
                        if len(bt_data) > 0:
                            #Send data to main
                            #self.to_controller('MANUAL',bt_data)
                            print(f'BTSERVER: received {bt_data}')
                    except IOError:
                        self.close_connection()
                        state = 'NO_CONN'
                        self.to_controller('CONNECTION_BROKEN')
                        self.hello()

                elif state == 'MK_CONN':
                    if self.connect_to_app(): # this may be blocking
                        state = 'BT_CONN'

                # This state should never run! See bools at top of loop.
                elif state == 'NO_CONN':
                    print('BTSERVER: wtf did you do man?!?!')



            else:
                time.sleep(period)

        print('btserver loop done!')
