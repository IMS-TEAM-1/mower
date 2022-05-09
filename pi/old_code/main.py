
from time import sleep

from arduino import Arduino
from camera  import Camera
from backend import Backend
from btserver import Btserver

import settings as s




if __name__ == '__main__':

    print('main runs')

    # Create instances
#    ard     = Arduino(s.ARDUINO_SERIAL_DEV,
#                      s.ARDUINO_SERIAL_BAUD,
#                      s.ARDUINO_SERIAL_TIMEOUT)

#    cam     = Camera(s.CAMERA_SCREENSHOT_RESOLUTION,
#                     s.CAMERA_DIRECTORY)

    backend = Backend(s.BACKEND_URI,
                      s.BACKEND_PORT)

    btserver = Btserver(s.BLUETOOTH_UUID)

    # state = "STANDBY"

    # Backend test

#    hello = str(backend.request_test_hello_world())
#    print(hello)
#    if hello == "hello world":
#        ard.hello()
#        sleep(1)
#        ard.send_message("AUTONOMOUS")

    # Start communicating                   
    # ard.hello()

#    btserver.connect_to_app()
#    bluetooth_active = True


    while True:
        #Get state from backend STANDBY/AUTONOMOUS/MANUAL
        #state = backend.request_state()
        state = "MANUAL"
        #print(state)

        #MANUAL
        if (state == "MANUAL"):
            btserver = Btserver(s.BLUETOOTH_UUID)
            btserver.connect_to_app()
            try:
                while(state == "MANUAL"):
                    data = btserver.recieve_message_from_app()
                    if (len(data) != 0):
                        btserver.send_message(data)
                        #ard.send_message(data.decode('UTF-8'))
                    sleep(0.5)
                    #state = backend.request_state()
                    print(state)

            except IOError:
                bluetooth_active = False
                btserver.close_connection()
                pass
        sleep(0.5)

        #AUTONOMOUS
        #print(ard.receive_message())
#        if ard.receive_message() == "CAPTURE":
#            Camera.open_sleep_capture_close(s.CAMERA_SCREENSHOT_RESOLUTION, s.CAMERA_DIRECTORY)
#            #cam.capturePicture()
#            ard.send_message("CAPTURE:ack")
#            print("CAPTURE:ack sent")
#            #print(ard.receive_message())
#        sleep(1)

        #STANDBY

    #     ard.post()
    #     sleep(1)
    #     #backendPost()
    #     #backendRequest()
    #     #arduinoRequest()
    #     #bluetoothTick()


