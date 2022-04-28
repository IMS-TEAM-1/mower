"""
This is the camera module.
"""
from time import sleep
import os

import settings as s

if s.PIZERO:
    from picamera import PiCamera


# Eventually we will want to have rapid-captuning, as described here:
# https://picamera.readthedocs.io/en/release-1.13/recipes2.html


class Camera:
    """
    This class is for the raspberry pi camera.
    Maybe non-python camera usage may be tested too
    if an easy example that works on the first try
    can be located.
    """
    def __init__(self, resolution, savedir):
        self.resolution = resolution
        self.savedir    = savedir

        if s.PIZERO:
            self.__cam = PiCamera()
            self.__cam.resolution = resolution
        else:
            pass

    def __del__(self):
        if s.PIZERO:
            self.__cam.close()
        else:
            pass

    def capture(self, img_name):
        """
        Take picture with the camera. Neither opens nor
        closes the connection to it.
        """
        sleep(2)
        path = os.path.join(s.CAMERA_DIRECTORY, img_name)
        if s.PIZERO:
            self.__cam.capture(path)
        else:
            print("Camera module: fake image at\n\t"
                    + path)
            
    def capturePicture(self):
        camera = PiCamera()
        camera.resolution = (1024, 768)
        sleep(1)
        camera.capture('/home/pi/image.jpg')
        camera.close

    @staticmethod
    def open_sleep_capture_close(res, directory):
        """
        Should run the constructor, take a picture and close
        the camera when done.
        """
        if s.PIZERO:
            cam = PiCamera()
            cam.resolution = res
            sleep(2)
            #path = os.path.join(directory, img_name)
            cam.capture('/home/pi/pics/image.jpg')
            cam.close()
        else:
            sleep(2)
