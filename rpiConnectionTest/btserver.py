"""
Bluetooth
"""

from bluetooth import*

import settings as s

class Btserver:
    
    def __init__(self, uuid):
        self.server_socket = BluetoothSocket(RFCOMM)
        self.server_socket.bind(("",PORT_ANY))
        self.server_socket.listen(1)
        self.port = self.server_socket.getsockname()[1]
        
        advertise_service( self.server_socket, "panzermower",
                   service_id = uuid,
                   service_classes = [ uuid, SERIAL_PORT_CLASS ],
                   profiles = [ SERIAL_PORT_PROFILE ], 
                    )
        
    def __del__(self):
        pass
    
    def connect_to_app(self):
        self.client_socket,self.client_info = self.server_socket.accept()
        print("connected")
        
    def send_message(self, message):
        #ard.send_message(message.decode('UTF-8'))
        print(message)
    
    def recieve_message(self):
        pass
    
    def recieve_message_from_app(self):
        data = self.client_socket.recv(1024)
        return data
    
    def close_connection(self):
        client_socket.close()
        print("disconnected")
        server_socket.close()





