from bluetooth import*
from arduino import Arduino

import settings as s

server_socket = BluetoothSocket(RFCOMM)
server_socket.bind(("",PORT_ANY))
server_socket.listen(1)

ard     = Arduino(s.ARDUINO_SERIAL_DEV,
                      s.ARDUINO_SERIAL_BAUD,
                      s.ARDUINO_SERIAL_TIMEOUT)


port = server_socket.getsockname()[1]

uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"

advertise_service( server_socket, "panzermower",
                   service_id = uuid,
                   service_classes = [ uuid, SERIAL_PORT_CLASS ],
                   profiles = [ SERIAL_PORT_PROFILE ], 
                    )

client_socket,client_info =server_socket.accept()
print("connected")
try:
    while True:
        data = client_socket.recv(1024)
        if len(data) == 0: break
        else:
            ard.send_message(data.decode('UTF-8'))
            #send_message(data)


except IOError:
    pass


client_socket.close()
print("disconnected")
server_socket.close()