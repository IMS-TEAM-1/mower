"""
This is the camera module. It is responsible for taking a pictures when it gets commands from the arduino.
It also gets the picture resolution and the directory to where to store the picture.
"""
import time
import os

from threading import Thread
from queue     import Queue

import settings as s
if s.PIZERO:
    from picamera import PiCamera



# Eventually we will want to have rapid-captuning
# (if possible), as described here:
# https://picamera.readthedocs.io/en/release-1.13/recipes2.html


class Camera(Thread):
    """
    This class is for the raspberry pi camera.
    Maybe non-python camera usage may be tested too
    if an easy example that works on the first try
    can be located.
    """
    def __init__(self, resolution, savedir, q_to_controller):
        Thread.__init__(self)
        self.resolution         = resolution
        self.savedir            = savedir
        self.orders             = Queue(maxsize = 10)
        self.q_to_controller    = q_to_controller

    def __del__(self):
        pass

    def capture(self, img_name):
        """
        Take picture with the camera. Neither opens nor
        closes the connection to it.

        RNR5
        """
        if s.PIZERO:
            self.__cam = PiCamera()
            self.__cam.resolution = self.resolution

        time.sleep(2)

        path = os.path.join(s.CAMERA_DIRECTORY, img_name)

        if s.PIZERO:
            self.__cam.capture(path)
            self.__cam.close()
            return path
        else:
            print('Camera module: fake image at\n\t'
                    + path)
            return path

    def order(self, message, payload = None):
        """
        See arduino module for usage.
        """
        self.orders.put( (message, payload) )

    def to_controller(self, message, payload = None):
        """
        See arduino module for usage.
        """
        self.q_to_controller.put( (self, message, payload) )


    def run(self):

        running = True

        while running:
            (message, payload) = self.orders.get()

            if message == 'CAPTURE':
                filename = 'CAPTURED_PIC.jpg' if (payload is None) \
                            else payload
                path = self.capture(filename)
                self.to_controller('CAPTURE_DONE', path)


            if message == 'EXIT':
                running = False
                print('camera quits!')

        print('camera loop done!')
