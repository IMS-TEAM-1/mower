"""
This is the backend module. Communication with our
server should be handled through this module.
It both sends and receives certain types of communication.
"""
import requests
import json
import base64

import time
import os       as os

from threading import Thread
from queue     import Queue


class Backend(Thread):
    """
    This class is meant to communicate with our backend.
    Check the settings file for constants used.
    """
    def __init__(self, adr, port, q_to_controller):
        Thread.__init__(self)
        self.backend_adr            = adr
        self.backend_service_port   = port
        self.base_uri               = f'http://{adr}:{port}/'
        self.orders                 = Queue(maxsize = 10)
        self.q_to_controller        = q_to_controller

    def __del__(self):
        # terminate connection to backend

        pass
    def can_reach_backend(self, timeout_sec=2):
        """
        Testing function if backend is responsive. Run before communication.
        This test should avoid crashes.
        """
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

    def get_user_json(self):
        get_uri  = f'{self.base_uri}users'
        # print(f'\tgetting request: {get_uri}')
        response = requests.get(get_uri).json()
        print(response)
        return response


    def get_state(self):
        """
        Get state from backend
        """
        get_uri     = self.base_uri + 'mowers'
        print(f'\tgetting request: {get_uri}')
        mower       = requests.get(get_uri)
        mower_info  = json.loads(mower.text)[0]
        be_state    = mower_info['state']

        print(f'Backend reports state {be_state}')
        return be_state


    def encode_picture_to_base64(self, filepath):
        """
        Convert picture to base64
        """
        with open(filepath, 'rb') as image_file:
            data = base64.b64encode(image_file.read())
            #print(data)
        return data.decode('utf-8')


    def post_pos(self, x, y):
        """
        Send position of mower to the backend.
        """
        data = {'key1' : x,
                'key2' : y,
                }
        get_uri  = self.base_uri + '/mowers/1/locations'
        requests.post(get_uri, data = data)


    def post_pic(self, x, y, pic64):
        """
        send picture and position of mower to the backend
        """
        data = {"x" : x,
                "y" : y,
                "image" : pic64 }
        get_uri  = self.base_uri + '/mowers/1/images'
        requests.post(get_uri, data = data)


    def order(self, message, payload = None):
        """
        Master tells this module what to do.
        """
        self.orders.put( (message, payload) )

    def to_controller(self, message, payload = None):
        """
        Report to master.
        """
        self.q_to_controller.put( (self, message, payload) )


    def run(self):

        running = True

        period = 0.2
        period_for_backend_requests = 10.0
        time_left_until_be_state_req = period_for_backend_requests

        last_known_position = (0,0)


        while running:
            q_elem_available    = not self.orders.empty()
            time_for_be_req     = time_left_until_be_state_req <= 0.0

            if q_elem_available:
                (message, payload) = self.orders.get()

                backend_pinged = self.can_reach_backend(timeout_sec=2)

                if message == 'POS_DATA':
                    (x,y) = payload
                    last_known_position = (x,y)
                    if backend_pinged:
                        self.post_pos(x,y)
                        # print('Backend received position'
                        #       + f'({x},{y})')
                    else:
                        print('BACKEND: Not posting postion because ping failed.')


                elif message == 'SEND_IMG':
                    filepath = payload
                    b64_img  = self.encode_picture_to_base64(filepath)
                    pos      = last_known_position
                    if backend_pinged:
                        (image_x, image_y) = pos
                        self.post_pic(image_x, image_y, b64_img)
                    else:
                        print('BACKEND: Not posting postion because ping failed.')

                elif message == 'EXIT':
                    running = False
                    print('backend quits!')

                else:
                    print('Backend: unhandled message '
                          + f'({message},{payload})')

            elif time_for_be_req:
                # reset countdown:
                time_left_until_be_state_req = period_for_backend_requests
                backend_pinged = self.can_reach_backend(timeout_sec=2)
                if backend_pinged:
                    be_state = self.get_state()
                    self.to_controller('STATE', be_state)
                    time_left_until_be_state_req = period_for_backend_requests
                else:
                    print('BACKEND: No state gotten because of failed ping.')

            else:
                time.sleep(period)
                time_left_until_be_state_req -= period

        print('backend loop done!')
