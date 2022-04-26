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


    def post_penis_test(self):
        """
        Testing function.
        """
        self.post_penis('8========D')


    def post_penis(self, dickpick):
        """
        Sends a string to the backend server.
        """
        print("backend_hello(): not implemented")


    def ready(self):
        """
        Ready for what?
        """
        print("backend_ready(): not implemented")

    def post(self):
        """
        Una-bomber
        """
        print("backend_post(): not implemented")

    def request(self):
        """
        Gimme shit
        """
        print("backend_request(): not implemented")
