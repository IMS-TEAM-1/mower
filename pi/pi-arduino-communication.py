import serial
port = "/dev/ttyUSB0
ser = serial.Serial(port, 9600)
def tell(msg):
    msg = msg + '\n'
    x = msg.encode('ascii')
    ser.write(x)

def hear():
    msg = ser.read_until()
    mystring = msg.decode('ascii')
    return mystring

while True:
    val = input()
    print("thanks")
    tell(val)
    var = hear()
    print(var)
    