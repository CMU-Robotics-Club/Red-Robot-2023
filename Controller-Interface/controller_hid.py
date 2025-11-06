# USB Joystick Interface for 2025 Red Robot Hackathon
# Reads button and axis input from Logitech Gamepad F310
# Packs data to be sent over serial to NRF24L01+ USB adapter

import sys
import hid
import time
import random

from common import find_serial_port


def find_joystick():
    joystick_id = None
    for device in hid.enumerate():
        if device['product_string'] == 'Logitech Dual Action':
            joystick_id = (device["vendor_id"], device["product_id"])
            break
    if joystick_id is None:
        print('Could not find controller! Is the controller plugged in?')
        sys.exit(1)

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
            rand_bytes = random.randrange(0, 2**32).to_bytes(4, 'little')
            #rand_bytes = random.randbytes(4)
            packed_bytes = rand_bytes + bytes(joystick_state) + rand_bytes
            packed_bytes += bytes([sum(packed_bytes) % 256])
            radio.write(packed_bytes)

if __name__ == '__main__':
    main()
