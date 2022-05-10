
# Mower Pi Zero documentation

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

Also keeps track of the state the mower is currently set to (AUTONOMOUS, MANUAL etc.).


## `backend.py`

Server communication. Deals with uploading images to the server among other things. Also receiving position data.


## `camera.py`

Only deals with the queue as input. Get a message, take a picture. That's all.


## `keyb.py`

A module for dealing with input. From `stdin` normally, but could be from other handles. Set up to take debug sequences.



