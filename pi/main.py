"""
Main/Controller module
"""
# from posixpath  import split
from time       import sleep

from queue      import Queue
# from threading  import Thread

from arduino    import Arduino
from backend    import Backend
from btserver   import BtServer
from camera     import Camera
from keyb       import Keyb

import settings as s

def main():
    """
    This is the main function.
    It's loop is also known as 'Controller' in the modules
    which it administers.
    """

    print('main() runs')


    q_to_controller = Queue(maxsize = 20)

    # Create instances
    arduino = Arduino(s.ARDUINO_SERIAL_DEV,
                      s.ARDUINO_SERIAL_BAUD,
                      s.ARDUINO_SERIAL_TIMEOUT,
                      q_to_controller)

    camera  = Camera(s.CAMERA_SCREENSHOT_RESOLUTION,
                     s.CAMERA_DIRECTORY,
                     q_to_controller)

    backend = Backend(s.BACKEND_URI,
                      s.BACKEND_PORT,
                      q_to_controller)

    keyb    = Keyb(s.KEYB_HANDLE, # this is mostly stdin
                   q_to_controller)

    btserver = BtServer(s.BLUETOOTH_UUID,
                        q_to_controller)



    # Backend test
    # backend.get_user_json()

    # Initialize hardware (don't make connections!)
    arduino.hello()
    # btserver.hello()

    # Start the threads
    arduino.start()
    backend.start()
    # btserver.start()
    camera.start()
    keyb.start()



    # send initial orders
    arduino.order('AUTONOMOUS')



    # Variables used in main loop.
    waiting_for_camera = False

    running = True
    while running:
        ##############################
        ##                          ##
        ##       MAIN LOOP          ##
        ##                          ##
        ##############################
        """
        This is the controller main loop.

        Messages are sent from each submodule, which all have a
        handle of the queue q_to_controller. Blocks when waiting
        to read from the queue, reacts and then goes back to
        blocking on queue read.
        """

        # All messages should conform to this standard. If there's
        # nothing of importance to send then set payload = None.
        # See here and in in the Camera module for example.
        (source, message, payload) = q_to_controller.get()


        if source is arduino:
            print(f'main(): ARD says: {message}')

            if message == 'CAPTURE': # we hit an obstacle
                # arduino has stopped by itself
                camera.order( ('CAPTURE', 'lol.jpg') )
                waiting_for_camera = True

            if message == 'POS':
                (x, y) = payload
                # something like this?
                backend.order('POS_DATA', (x, y))


        if source is btserver:
            pass


        if source is camera:
            print(f'main(): CAM says: {message}')
            if message == 'CAPTURE_DONE':
                filepath = payload
                waiting_for_camera = False
                arduino.order('CAPTURE_DONE')
                backend.order('SEND_IMG', filepath)


        if source is backend:
            if message == 'GOTO_BT':
                print('Turning on BT controls')


        if source is keyb:
            if message == 'EXIT':
                print('main() got EXIT from keyb')
                arduino.order('EXIT')
                btserver.order('EXIT')
                camera.order('EXIT')
                backend.order('EXIT')
                keyb.order('EXIT')
                running = False

            elif message == 'test1':
                pass

            elif message == 'test2':
                pass

            elif message == 'test3':
                pass

            elif message == 'test4':
                pass

        else:
            print( 'main() unhandled message:')
            print(f'    ({source}, {message}, {payload})')

    print('joining threads')
    arduino.join()
    backend.join()
    # btserver.join()
    camera.join()
    keyb.join()
    print('exiting')


if __name__ == '__main__':
    main()



