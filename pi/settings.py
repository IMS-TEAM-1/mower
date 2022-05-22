"""
Constants to be used for the RPiZ program.
"""
import os
import sys

# This is a define that will enable testing to be done
# on computers that are NOT raspberry pis
PIZERO  = True
ON_UNIX = os.name == 'posix'


#########################
#########################
# RASPBERRY PY          #
#########################
#########################
RPI_UPDATE_FREQUENCY_HERTZ = 20




#########################
#########################
# ARDUINO               #
#########################
#########################
ARDUINO_SERIAL_BAUD = 57600

ARDUINO_SERIAL_DEV  = '/dev/ttyUSB0' if ON_UNIX \
                else 'c:\\loldunno-this-is-windows'

ARDUINO_SERIAL_TIMEOUT = 1
ARDUINO_VALID_STATES = ['AUTONOMOUS','MANUAL','STANDBY','DIAGNOSTIC']




#########################
#########################
# CAMERA                #
#########################
#########################
CAMERA_DIRECTORY = '/home/pi/pics/' if PIZERO  \
              else '~/pics/'        if ON_UNIX \
              else 'c:\\pr0n\\'

CAMERA_SCREENSHOT_RESOLUTION = (1024, 768)


#########################
#########################
# KEYBOARD              #
#########################
#########################
KEYB_HANDLE = sys.stdin



#########################
#########################
# BACKEND               #
#########################
#########################
BACKEND_ADR  = '13.48.151.7'
BACKEND_PORT = 8080


#########################
#########################
# MOBILE                #
#########################
#########################
MOBILE_BLUETOOTH_DUMMY = 'some configuration loldunno'


#########################
#########################
# BTSERVER              #
#########################
#########################
BLUETOOTH_UUID = '94f39d29-7d6d-437d-973b-fba39e49d4ee'
#BLUETOOTH_UUID = '0000110a-0000-1000-8000-00805f9b34fb'
