# USB Joystick Interface for 2023 Red Robot Hackathon
# Reads button and axis input from Logitech Gamepad F310
# Packs data to be sent over serial to NRF24L01+ USB adapter

import sys
import glob
import hid
import time
import serial
import serial.tools.list_ports
import random

def find_serial_port():
    if sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.usbserial-*')
    elif sys.platform.startswith('win'):
        ports = filter(lambda x: x.pid == 29987, serial.tools.list_ports.comports())
        ports = list(map(lambda x: x.name, ports))
    elif sys.platform.startswith('linux'):
    	ports = ["/dev/ttyUSB0"]
    else:
        raise Exception('UNKNOWN OPERATING SYSTEMS')

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

def find_joystick():
    joystick_id = (0x046d, 0xc21d)
    print("Found controller with id", joystick_id)
    
    joystick = hid.device()
    joystick.open(joystick_id[0], joystick_id[1])
    joystick.set_nonblocking(True)

    return joystick

def main():
    radio = find_serial_port()
    joystick = find_joystick()

    left_x, left_y, right_x, right_y, buttons1, buttons2 = 127, 127, 127, 127, 0, 0

    last_t = time.time()
    while True:
        report = joystick.read(64)
        if report:
            left_x, left_y, right_x, right_y, buttons1, buttons2, padding1, padding2 = report

            left_y  = 255 - left_y
            right_y = 255 - right_y

        t = time.time()
        if t - last_t >= 0.03:
            joystick_state = [left_x, left_y, right_x, right_y, buttons1, buttons2]

            last_t = t
            rand_bytes = random.randbytes(4)
            packed_bytes = rand_bytes + bytes(joystick_state) + rand_bytes
            packed_bytes += bytes([sum(packed_bytes) % 256])
            radio.write(packed_bytes)

if __name__ == '__main__':
    main()
