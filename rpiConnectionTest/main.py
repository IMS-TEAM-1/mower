
from time import sleep

from arduino import Arduino
from camera  import Camera
from backend import Backend
from btserver import Btserver

import settings as s




if __name__ == '__main__':

    print('main runs')

    # Create instances
    ard     = Arduino(s.ARDUINO_SERIAL_DEV,
                      s.ARDUINO_SERIAL_BAUD,
                      s.ARDUINO_SERIAL_TIMEOUT)

#    cam     = Camera(s.CAMERA_SCREENSHOT_RESOLUTION,
#                     s.CAMERA_DIRECTORY)

    backend = Backend(s.BACKEND_URI,
                      s.BACKEND_PORT)
    
    btserver = Btserver(s.BLUETOOTH_UUID)

    # state = MANUAL

    # Backend test

    hello = str(backend.request_test_hello_world())
    print(hello)
    if hello == "hello world":
        ard.hello()
        sleep(1)
        ard.send_message("AUTONOMOUS")
    
    

    # Start communicating                   
    # ard.hello()
    
    btserver.connect_to_app()
    bluetooth_active = True

    while True:
        while(bluetooth_active): #ersätt med status från backend
            data = btserver.recieve_message_from_app()
            if (len(data) != 0):
                #btserver.send_message(data)
                ard.send_message(data.decode('UTF-8'))
        
        
        #print(ard.receive_message())
#        if ard.receive_message() == "CAPTURE":
#            Camera.open_sleep_capture_close(s.CAMERA_SCREENSHOT_RESOLUTION, s.CAMERA_DIRECTORY)
#            #cam.capturePicture()
#            ard.send_message("CAPTURE:ack")
#            print("CAPTURE:ack sent")
#            #print(ard.receive_message())
#        sleep(1)

    #     ard.post()
    #     sleep(1)
    #     #backendPost()
    #     #backendRequest()
    #     #arduinoRequest()
    #     #bluetoothTick()


