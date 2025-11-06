import sys
from glob import glob
import serial
import serial.tools.list_ports


def find_serial_port():
    if sys.platform.startswith('darwin'):
        ports = glob('/dev/tty.usbserial-*')
    elif sys.platform.startswith('win'):
        ports = filter(lambda x: x.pid == 29987, serial.tools.list_ports.comports())
        ports = list(map(lambda x: x.name, ports))
    else: # assume linux
        ports = glob('/dev/ttyACM*') + glob('/dev/ttyUSB*')

    radio = None

    for port in ports:
        try:
            print(port)
            radio = serial.Serial(port, baudrate=115200)
        except (OSError, serial.SerialException):
            pass

    if radio is None:
        print('Could not find radio! Is the dongle plugged in?')
        sys.exit(1)

    return radio
