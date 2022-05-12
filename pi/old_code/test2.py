import serial
from time import sleep
# from picamera import PiCamera

def sendMessage(msg):
    ser.write((msg + "\n").encode('ascii'))

def recieveMessage():
    line = ser.readline()
    line = str(line.decode('utf-8').strip())
    return line
    
def capturePicture():
    # if recieveMessage == 'CAPTURE':
    #     camera = PiCamera()
    #     camera.resolution = (1024, 768)
    #     sleep(2)
    #     camera.capture('/home/pi/image.jpg')
    #     camera.close()
        print("picture taken")
        
def arduinoHello():
    sendMessage('Hello')
    while recieveMessage() != 'Hello:ack':
        print("arduinoHello(): not ready")
        sendMessage('Hello')
        sleep(2)
    
    print("arduinoHello(): ready")
      
def arduinoRdy():
    sendMessage('rdy')
    while recieveMessage() != 'rdy:ack':
        print("arduinoRdy(): not ready")
        sendMessage('rdy')
        sleep(2)
    
    print("arduinoRdy(): ready")

def backendHello():
    print("backendHello(): not implemented")
    
def backendRdy():
    print("backendRdy(): not implemented")
    
def backendPost():
    print("backendPost(): not implemented")
    
def backendRequest():
    print("backendRequest(): not implemented")
    
def arduinoPost():
    #print("arduinoPost(): not implemented")
    keyboard = input("write your command: ")
    sendMessage(keyboard)
    sleep(0.1)
    print(recieveMessage())
    
    
    
if __name__ == '__main__':
    ser = serial.Serial('/dev/ttyUSB0', 115200, timeout = 1)
    ser.reset_input_buffer()
    arduinoHello()
    #arduinoRdy()


    while True:
        
        #backendPost()
        #backendRequest()
        arduinoPost()
        #arduinoRequest()
        #bluetoothTick()
        sleep(1)
