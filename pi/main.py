
from time import sleep

from arduino import Arduino
from camera  import Camera
from backend import Backend

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

    # state = MANUAL

    # Start communicating
    ard.hello()

    while True:

        ard.post()
        sleep(1)
        #backendPost()
        #backendRequest()
        #arduinoRequest()
        #bluetoothTick()


