"""
This is the backend module.
"""
import requests
import json

# Create a constant for our url, notice the slash / in the end.

# make the request and convert it to json object
# here we concatenate the base url with the object we want to request, "mowers"

import settings as s


class Backend:
    """
    This class is meant to communicate with our backend.
    Check the settings file for constants used.
    """
    def __init__(self, uri, port):
        self.base_uri  = f'http://{uri}:{port}/'
        # create object
        # and maybe establish a connection?

    def __del__(self):
        # terminate connection to backend
        pass

    def get_user_json(self):
        get_uri  = self.base_uri + 'users'
        print(f'\tgetting request: {get_uri}')
        response = requests.get(get_uri).json()
        print(response)
        return response

    def get_state(self):
        """
        get state from backend
        """
        get_uri  = self.base_uri
        print(f'\tgetting request: {get_uri}')
        beState = requests.get(get_uri).json()
        print(beState)
        return beState


    def decode_picture_to_base64(self):
        """
        Convert picture to base64
        """
        with open('/home/pi/pics/img',"rb") as image_file:
            data = base64.b64encode(image_file.read())
            #print(data)
        return data.decode('utf-8')


    def post_pos(self, position):
        """
        sent position of mower to the backend

        """
        x = position.split(":")[1]
        y = position.split(":")[2]

        data = {'key1': x,
        'key2': y,
        'api_paste_format':'python'
        }
        
        get_uri  = self.base_uri + '/mowers/7/locations'
        requests.post(get_uri, data = data)

    
