"""
This is the backend module.
"""


import settings as s


class Backend:
    """
    This class is meant to communicate with our backend.
    Check the settings file for constants used.
    """
    def __init__(self, uri, port):
        self.uri  = uri
        self.port = port
        # create object
        # and maybe establish a connection?

    def __del__(self):
        # terminate connection to backend
        pass

    def hello(self):
        """
        Initial handshake
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
