"""
This is the keyboard module. To read from the handle
provided (from settings).
Often the handle is stdin, but others may be used.

Mainly a way to run sequences of test code.
"""
import select
import time

from threading import Thread
from queue     import Queue

class Keyb(Thread):
    """
    This class is for the raspberry pi camera.
    Maybe non-python camera usage may be tested too
    if an easy example that works on the first try
    can be located.
    """
    def __init__(self, handle, q_to_controller):
        Thread.__init__(self)
        self.handle             = handle
        self.orders             = Queue(maxsize = 10)
        self.q_to_controller    = q_to_controller

    def __del__(self):
        pass

    def order(self, message, payload = None):
        """
        See arduino module for usage.
        """
        self.orders.put( (message, payload) )

    def to_controller(self, message, payload = None):
        """
        See arduino module for usage.
        """
        self.q_to_controller.put( (self, message, payload) )


    def run(self):

        running = True
        period  = 0.1

        while running:
            handle_available    = select.select([self.handle, ],
                                                [], [], 0.0)[0]
            q_elem_available    = not self.orders.empty()

            if q_elem_available:
                (message, payload) = self.orders.get()
                if message == 'EXIT':
                    print('keyb quits!')
                    running = False
                else:
                    print('keyb: unhandled order {message}')

            elif handle_available:
                line = self.handle.readline().strip()
                print(f'keyb says <<<{line}>>>')
                if line in ('EXIT', 'exit'):
                    self.to_controller('EXIT')
                else:
                    self.to_controller(line)

            else:
                time.sleep(period)

        print('keyb loop done!')
