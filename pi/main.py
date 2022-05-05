
from posixpath import split
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

    cam     = Camera(s.CAMERA_SCREENSHOT_RESOLUTION,
                     s.CAMERA_DIRECTORY)

    backend = Backend(s.BACKEND_URI,
                      s.BACKEND_PORT)
     
    btserver = Btserver(s.BLUETOOTH_UUID)


    # Backend test
    backend.get_user_json()

    # Start communicating
    ard.hello()

    while True:

         ard.send_state(backend.get_state())

         if backend.get_state() == "AUTONOMOuS" and ard.receive_message() == "AUTONOMOuS:ack":
            if ard.receive_message() == "CAPTURE":
                 Camera.open_sleep_capture_close(s.CAMERA_SCREENSHOT_RESOLUTION, s.CAMERA_DIRECTORY, "img")
                 backend.post(backend.decode_picture_to_base64)
                 ard.send_message("CAPTURE:ack")

            if (ard.receive_message()).split(":")[0] == "pos":
                backend.post_pos(ard.receive_message())



        
         

         


         #bluetoothTick()


