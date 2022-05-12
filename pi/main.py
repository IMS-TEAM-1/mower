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
    #arduino = Arduino(s.ARDUINO_SERIAL_DEV,
    #                  s.ARDUINO_SERIAL_BAUD,
    #                  s.ARDUINO_SERIAL_TIMEOUT,
    #                  q_to_controller)

    camera  = Camera(s.CAMERA_SCREENSHOT_RESOLUTION,
                     s.CAMERA_DIRECTORY,
                     q_to_controller)

    backend = Backend(s.BACKEND_ADR,
                      s.BACKEND_PORT,
                      q_to_controller)

    keyb    = Keyb(s.KEYB_HANDLE, # this is mostly stdin
                   q_to_controller)

    btserver = BtServer(s.BLUETOOTH_UUID,
                        q_to_controller)



    # Backend test
    # backend.get_user_json()

    # Initialize hardware (don't make connections!)
    # arduino.hello()
    btserver.hello()

    # Start the threads
    #arduino.start()
    #backend.start()
    btserver.start()
    camera.start()
    keyb.start()

    #Run while loop
    running = True

    # send initial orders
    #arduino.order('STATE','STANDBY')
    
    state = 'STANDBY'
    #state = 'NO_CONN'
    # Variables used in main loop.
    waiting_for_camera = False

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


        #if source is arduino:
            # print(f'main(): ARD says: {message}')

        #    if message == 'CAPTURE': # we hit an obstacle
                # arduino has stopped by itself
        #        camera.order( ('CAPTURE', 'image.jpg') )
        #        waiting_for_camera = True

        #    if message == 'POS':
        #        (x, y) = payload
                # something like this?
        #        backend.order('POS_DATA', (x, y))

        #elif
        if source is btserver:
            print('main() got btserver message!')
            if message == 'MANUAL':
                if payload == 'FORWARD':
                    arduino.order('MANUAL:FORWARD')
                if payload == 'BACKWARD':
                    arduino.order('MANUAL:BACKWARD')
                if payload == 'LEFT':
                    arduino.order('MANUAL:LEFT')
                if payload == 'RIGHT':
                    arduino.order('MANUAL:RIGHT')


        elif source is camera:
            print(f'main(): CAM says: {message}')
            if message == 'CAPTURE_DONE':
                filepath = payload
                waiting_for_camera = False
                arduino.order('CAPTURE_DONE')
                backend.order('SEND_IMG', filepath)


        elif source is backend:

            print('main() got backend message!')

            if message == 'GOTO_BT':
                btserver.order('BACKEND_SAYS_ACTIVATE_BT')
                print('Turning on BT controls')

            elif message == 'STATE':
                ok_state = state in s.ARDUINO_VALID_STATES
                if ok_state:
                    if payload != state:
                        arduino.order('SET_STATE', payload)
                        state = payload
                        if state == 'MANUAL':
                            arduino.order('MANUAL:NONE')
                            btserver.order('BACKEND_SAYS_ACTIVATE_BT')
                            print('Turning on BT controls')
                        elif state == 'AUTONOMOUS':
                            arduino.order('AUTONOMOUS')
                        elif state == 'STANDBY':
                            arduino.order('STANDBY')
                        elif state == 'DIAGNOSTIC':
                            arduino.order('DIAGNOSTIC')
                    else:
                        print(f'main(): state was already {payload}')
                else:
                    print(f'main(): invalid state: {payload}')



        elif source is keyb:
            if message == 'EXIT':
                print('main() got EXIT from keyb')
                #arduino.order('EXIT')
                btserver.order('EXIT')
                camera.order('EXIT')
                backend.order('EXIT')
                keyb.order('EXIT')
                running = False

            elif message == 'test1':
                arduino.order('AUTONOMOUS')
                pass

            elif message == 'test2':
                backend.order('GET_STATE')
                pass

            elif message == 'test3':
                btserver.order('BACKEND_SAYS_ACTIVATE_BT')
                pass

            elif message == 'test4':
                btserver.order('BACKEND_SAYS_DEACTIVATE_BT')
                pass

        else:
            print( 'main() unhandled message:')
            print(f'    ({source}, <<<{message}>>>, <<<{payload}>>>)')

    print('joining threads')
    # arduino.join()
    # backend.join()
    btserver.join()
    camera.join()
    keyb.join()
    print('exiting')


if __name__ == '__main__':
    main()



