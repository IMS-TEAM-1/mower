
# Mower Pi Zero documentation


## `main.py`

The file `main.py` is what starts all the sub-modules and then starts all the threads for the modules. Most of the sub-systems deal with multiple inputs. They might be dealing with reading some handle/socket as the same time as they're waiting for queued messages from the main loop.

To understand the main-loop code, it's similar to this:

```{.python}
def main():
    sub1 = Sub1()
    sub2 = Sub2()
    sub3 = Sub3()

    sub1.init() # initialize hardware
    sub2.init()
    sub3.init()

    sub1.start() # start up threads
    sub2.start()
    sub3.start()


    while True:

        (src,msg,payload) = queue.get() # blocking read

        if src == sub1:
            if msg == 'TYPE1':
                do_thing_1()
            if msg == 'TYPE2':
                do_thing_2()
                sub2.order('TYP2_CONSEQUENCE')

        if src == sub2:
            # the rest of the handlers
            pass
```


## `arduino.py`

Reads and writes over serial connection to the Arduino, which manages motors, top LEDs, gyroscope and other things.
```
def send_serial(self, message):
    print(f'ARDUINO <==== <<{message}>>')
    self.ser.write((message + '\r\n').encode('ascii'))
```

 is responsible of sending the data over to arduino via serial connection.

```
def receive_serial(self):
    line = self.ser.readline()
    line = str(line.decode('utf-8').strip())
    self.ser.reset_input_buffer()

    print(f'ARDUINO ====> <<{line}>>')
    return line
```

 is responsible of receiving data from the arduino over the serial connection. it also return a string with stripped newline characters.


It also keeps track of the state the mower is currently set to (AUTONOMOUS, MANUAL etc.).

It also contains the first connection check with arduino. Both the mower and raspberry pi should wait until arduino sends back that it is ready.
```
def hello(self):
        greeting        = send_dict['Hello']
        self.send_serial(greeting)
        time.sleep(0.050) # let's wait before reading
        received        = self.receive_serial()
        expected_reply  = greeting + send_dict['ack']

        while received != expected_reply:
            print('arduinoHello(): not ready, '
                  + f'instead got <<<{received}>>>')
            self.send_serial(send_dict['Hello'])
            time.sleep(2)
            received = self.receive_serial()
        print('arduinoHello(): ready')
```

## `backend.py`

Server communication. Deals with uploading images to the server among other things. Also receiving position data.

```
def can_reach_backend:
    tries       = 1
    cmd         = f'ping -c {tries}'            + \
                   f' -w {timeout_sec}'         + \
                   f' -O {self.backend_adr}'    + \
                   ' 1> /dev/null'
     ret = os.system(cmd)
     if ret == 0:
         # print(f'BACKEND: could not reach {self.backend_adr} for ping.')
            pass
     return not bool(ret)
```

 Testing function if backend is responsive. Run before communication.
 This test should avoid crashes.

```
def get_state:
     get_uri     = self.base_uri + 'mowers'
     print(f'\tgetting request: {get_uri}')
     mower       = requests.get(get_uri)
     mower_info  = json.loads(mower.text)[0]
     be_state    = mower_info['state']

     print(f'Backend reports state {be_state}')
     return be_state
```

 gets the state of the mower from backend and returns it, then the state should be send to the arduino via "arduino.py" module.

```
def encode_picture_to_base64:
 with open(filepath, 'rb') as image_file:
         data = base64.b64encode(image_file.read())
 return data.decode('utf-8')
         
 this functions is responsible of converting the taken image to base64 to further send it to the backend via def post_pic function.
```

```
def post_pos:
 data = {'key1' : x,
         'key2' : y,
         }
 get_uri  = self.base_uri + '/mowers/1/locations'
 requests.post(get_uri, data = data)
```

 this function is responsible of posting the mowers location to the backend, the location is collected from the serial commuinication between the arduino and the raspberry pi in the "arduino.py" module.

```
def post_pic:
 data = {"x" : x,
         "y" : y,
         "image" : pic64 }
 get_uri  = self.base_uri + '/mowers/1/images'
 requests.post(get_uri, data = data)
```

 this function is responsible of posting the picture in base64 to the backend along side of the position of where the picture was taken.

## `camera.py`
```
 def capture(self, img_name):
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
         print('Camera module: fake image at\n\t' + path)
         return path
```

Only deals with the queue as input. Get a message, take a picture. It is responsible for taking a pictures when it gets commands from the arduino.
It also gets the picture resolution and the directory to where to store the picture.



## `keyb.py`

A module for dealing with input. From `stdin` normally, but could be from other handles. Set up to take debug sequences.

This is the keyboard module. To read from the handle
provided (from settings).
Often the handle is stdin, but others may be used.

Mainly a way to run sequences of test code.

## `settings.py`

This file is self-explanatory, it contains a multitude of defines and other data types used within the software to avoid magic numbers and improve readability.

## `btserver.py`
```
def hello(self):
     self.server_socket = bt.BluetoothSocket(bt.RFCOMM)
     self.server_socket.bind((self.uuid, bt.PORT_ANY))
     self.server_socket.listen(1)
     self.server_port = self.server_socket.getsockname()[1]

     bt.advertise_service(
         self.server_socket,
         "panzermower",
         service_id      = self.uuid,
         service_classes = [ self.uuid, bt.SERIAL_PORT_CLASS ],
         profiles        = [ bt.SERIAL_PORT_PROFILE ] )
```
 this function is responsible of Initialize the BT to allow connection with the app.
 Advertises the BT device as connectable server.


```
def connect_to_app(self):
     self.client_socket, self.client_info = self.server_socket.accept()
     print(f'BTSERVER: ({self.client_socket},{self.client_info}) connected')
     return True
```
 this function is responsible for creating a socket connection to the application.

```
def close_connection(self):
     self.client_socket.close()
     self.server_socket.close()
     print('BTSERVER: close_connection()')
```
 this function is responsible of closing socket connection.
