"""
This is the backend module. Communication with our
server should be handled through this module.
It both sends and receives certain types of communication.
"""
import requests
import json
import base64

import time
import os

import settings as s

from threading import Thread
from queue     import Queue

import settings as s


class Backend(Thread):
    """
    This class is meant to communicate with our backend.
    Check the settings file for constants used.
    """
    def __init__(self, uri, port, q_to_controller):
        Thread.__init__(self)
        self.base_uri           = f'http://{uri}:{port}/'
        self.orders             = Queue(maxsize = 10)
        self.q_to_controller    = q_to_controller

    def __del__(self):
        # terminate connection to backend
        pass


    def get_user_json(self):
        get_uri  = f'{self.base_uri}users'
        print(f'\tgetting request: {get_uri}')
        response = requests.get(get_uri).json()
        print(response)
        return response


    def get_state(self):
        """
        Get state from backend
        """
        get_uri  = self.base_uri
        print(f'\tgetting request: {get_uri}')
        be_state = requests.get(get_uri).json()
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
                'api_paste_format' : 'python' }
        get_uri  = self.base_uri + '/mowers/7/locations'
        requests.post(get_uri, data = data)


    # def post_pic(self, position, pic64):
    #     """
    #     send picture and position of mower to the backend
    #     """
    #     x = position.split(":")[1]
    #     y = position.split(":")[2]
    #     data = {"x" : x,
    #             "y" : y,
    #             "image" : pic64 }
    #     get_uri  = self.base_uri + '/mowers/1/images'
    #     requests.post(get_uri, data = data)


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

        while running:

            (message, payload) = self.orders.get()

            if message == 'POSITION':
                (x,y) = payload
                print('Backend received position'
                      + f'({x},{y})')
                self.post_pos(x,y)

            elif message == 'GET_STATE':
                be_state = self.get_state()
                self.to_controller('STATE', be_state)

            elif message == 'SEND_IMG':
                filepath = payload
                b64_img  = self.encode_picture_to_base64(filepath)
                ###
                ### <<< send image here >>>
                ###

            elif message == 'EXIT':
                running = False
                print('backend quits!')

            else:
                print('Backend: unhandled message '
                      + f'({message},{payload})')

        print('backend loop done!')
