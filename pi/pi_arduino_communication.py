import serial
from time import sleep
from picamera import PiCamera

port = "/dev/ttyUSB0"
ser = serial.Serial(port, 9600)

def tell(msg):
    msg = msg + '\n'
    x = msg.encode('ascii')
    ser.write(x)
    
def hear():
    msg = ser.read_until()
    #mystring = msg.decode('ascii')
    return msg

def capturePicture():
    camera = PiCamera()
    camera.resolution = (1024, 768)
    #camera.start_preview()
    #sleep(5)
    camera.capture('/home/pi/image.jpg')
    #camera.stop_preview()
    camera.close()

while True:
    if hear() == b'camera\r\n':
        capturePicture()
        print("picture taken")
    
