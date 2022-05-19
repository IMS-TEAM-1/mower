
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

def send_serial(self, message): is responsible of sending the data over to arduino via serial connection.
def receive_serial(self): is responsible of receiving data from the arduino over the serial connection. it also return a string with stripped newline characters.

Also keeps track of the state the mower is currently set to (AUTONOMOUS, MANUAL etc.).

It also contains the first connection check with arduino. Both the mower and raspberry pi should wait until arduino sends back that it is ready.


## `backend.py`

Server communication. Deals with uploading images to the server among other things. Also receiving position data.

def can_reach_backend: Testing function if backend is responsive. Run before communication.
This test should avoid crashes.

def get_state: gets the state of the mower from backend and store it, then the state should be send to the arduino via "arduino.py" module.

def encode_picture_to_base64: this functions is responsible of converting the taken image to base64 to further send it to the backend via def post_pic function.

def post_pos: this function is responsible of posting the mowers location to the backend, the location is collected from the serial commuinication between the arduino and the raspberry pi in the "arduino.py" module.

def post_pic: this function is responsible of posting the picture in base64 to the backend along side of the position of where the picture was taken.

## `camera.py`

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


