from time import sleep
from picamera import PiCamera


def capturePicture():
    camera = PiCamera()
    camera.resolution = (1024, 768)
    camera.start_preview()
    sleep(2)
    camera.capture('/home/pi/Desktop/image.jpg')
    camera.stop_preview()

if __name__ == '__main__':
    capturePicture()


