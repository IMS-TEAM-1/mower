
# Mower: Raspberry Pi Software Design Description

The sub-modules in this solution all have three functions:

    * `order(message, payload)`
    * `to_controller(message, payload)`
    * `run()`

and in many cases

    * `hello()`

The function `run()` is what's needed by the `Thread` class (called by the function `Thread.start()`).
The communication modules are all non-blocking, often making use of the function `os.select()` to check if there's any reading to be done from their sockets/handles.
The modules are controlled by the `main()` function, which are told through the function `order()` that each sub-module has.
When a sub-module receives a message it goes back to `main()` through the function `to_controller()`.

The function `hello()` is when an initial handshake makes sense.
Such as the first communication over a serial connection, or announcing the Bluetooth device as connectable.


This document is just an overview.
The functions themselves are all commented.
This document is just to help a reader quickly get into our code structure.



## `main.py`

The file `main.py` is what starts all the sub-modules and then starts all the threads for the modules. Most of the sub-systems deal with multiple inputs. They might be dealing with reading some handle/socket as the same time as they're waiting for queued messages from the main loop.

To understand the main-loop code, it's similar to this:

```{.python}
def main():
    sub1 = Sub1()
    sub2 = Sub2()
    sub3 = Sub3()

    sub1.hello() # initialize hardware
    sub2.hello()
    sub3.hello()

    sub1.start() # start up threads (function run())
    sub2.start()
    sub3.start()


    while True:

        (src,msg,payload) = queue.get() # blocking read

        if src == sub1:
            if msg == 'TYPE1':
                sub2.order('DO_THING_1')
            if msg == 'TYPE2':
                sub2.order('DO_THING_2', payload)
                sub3.order('TYPE2_CONSEQUENCE')

        elif src == sub2:
            # the rest of the handlers
            pass

        elif src == sub3:
            # more management
            pass

        else:
            print(f'main(): unknown message <<<{(src,msg,payload)}>>>')
```



## `arduino.py`

Reads and writes over serial connection to the Arduino, which manages motors, top LEDs, gyroscope and other things.
The actual serial Tx/Rx are done by these:

    * `send_serial(message: str)`
    * `receive_serial() -> str`

It also keeps track of the state the mower is currently set to (AUTONOMOUS, MANUAL etc.).

Because of communication problems (dropped characters) a quick fix changing the protocol from long strings like `'MANUAL:FORWARD'` to `'1'` was done.
This is why there's a `dict` in the top of the file.



## `backend.py`

Responsible for server communication.
Deals with uploading images to the server among other things.
Also receiving position data.

These are the unique functions provided:

    * `can_be_reached(timeout_sec = 2)`
    * `get_user_json() -> str`
    * `get_state() -> str`
    * `encode_picture_to_base64(filepath: str)`
    * `post_pos(x, y)`
    * `post_pix(x, y, pic64)`


Most of these are pretty self explanatory by their names and arguments, but these are some notes for better understanding:

The state is held by the backend, so it needs to be constantly polled for any changes.
If the mower is to change state (from `'AUTONOMOUS'` to `'MANUAL'` or similar) then that can only happen if the function `get_state()` gets a result different from what it previously had.

As part of the specification, uploading pictures needs to happen.
This is done in base64 format, together with coordinates, as can been seen by the function and argument names.



## `camera.py`

Only deals with the queue as input: get a message, take a picture.
Is done through its only unique function:

    * `capture(img_name)`

Its constructor takes the picture resolution and the directory to where to store it.
Sends a message up to the controller when done.



## `keyb.py`

A module for dealing with input. From `stdin` normally, but could be from other handles. Set up to take debug sequences.

This is the keyboard module. To read from the handle
provided (from settings).
Often the handle is stdin, but others may be used.

Mainly a way to run sequences of test code.




## `settings.py`

This file is self-explanatory, it contains a multitude of defines (in the C macro sense) and other data types used within the software to avoid magic numbers and improve readability.

Here are some lines, to give an understanding of what's stored in it:

```{.python}
ARDUINO_SERIAL_BAUD = 57600
CAMERA_SCREENSHOT_RESOLUTION = (1024, 768)
BLUETOOTH_UUID = '94f39d29-7d6d-437d-973b-fba39e49d4ee'
```



## `btserver.py`


These are the unique functions for this module:

    * `connect_to_app()`
    * `recieve_message_from_app()`
    * `close_connection()`

`hello()` is responsible for initializing the Bluetooth device and to advertise it as something that can be connected to.
In the same non-blocking nature as all the other modules it only reads a message when is ready.
If there's ever an exception once a connection has been established, then that will be caught, the server brought down and up again - restarting it with `hello()`.



