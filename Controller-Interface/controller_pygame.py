# USB Joystick Interface for 2023 Red Robot Hackathon
# Reads button and axis input from Logitech Gamepad F310
# Packs data to be sent over serial to NRF24L01+ USB adapter

import sys
import glob
import time
import serial
import serial.tools.list_ports
import random
import pygame

hat_dict = {(0,0): 8, (0,1): 0, (1,1): 1, (1,0): 2, (1,-1): 3, (0,-1): 4, (-1,-1): 5, (-1,0): 6, (-1,1): 7}

def find_serial_port():
    if sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.usbserial-*')
    elif sys.platform.startswith('win'):
        ports = filter(lambda x: x.pid == 29987, serial.tools.list_ports.comports())
        ports = list(map(lambda x: x.name, ports))
    else:
        raise Exception('TODO:')

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
    pygame.init()
    pygame.joystick.init()
    joystick_count = pygame.joystick.get_count()
    if joystick_count != 1:
        print("Could not find controller! ({}) Is the controller plugged in?".format(joystick_count))
        sys.exit(1)
    print("Found controller")
    joystick = pygame.joystick.Joystick(0)

    return joystick

def main():
    radio = find_serial_port()
    joystick = find_joystick()

    left_x, left_y, right_x, right_y, buttons1, buttons2 = 127, 127, 127, 127, 0, 0

    last_t = time.time()
    while True:
        for evt in pygame.event.get():
            pass
        t = time.time()
        if t - last_t >= 0.1:
            left_x = int(255 * ((joystick.get_axis(0) + 1.0) / 2.0))
            left_y = int(255 * ((joystick.get_axis(1) - 1.0) / -2.0))
            right_x = int(255 * ((joystick.get_axis(2) + 1.0) / 2.0))
            right_y = int(255 * ((joystick.get_axis(3) - 1.0) / -2.0))
            buttons1 = (joystick.get_button(3) << 7) | \
                       (joystick.get_button(1) << 6) | \
                       (joystick.get_button(0) << 5) | \
                       (joystick.get_button(2) << 4)

            try:
                buttons1 |= hat_dict[joystick.get_hat(0)]
            except:
                pass
            
            buttons2 = (joystick.get_button(9) << 7) | \
                       (joystick.get_button(8) << 6) | \
                       (joystick.get_button(7) << 5) | \
                       (joystick.get_button(6) << 4) | \
                       (joystick.get_button(5) << 1) | \
                       (joystick.get_button(4) << 0)
            
            joystick_state = [left_x, left_y, right_x, right_y, buttons1, buttons2]

            last_t = t
            rand_bytes = random.randrange(0, 2**32).to_bytes(4, 'little')
            #rand_bytes = random.randbytes(4)
            packed_bytes = rand_bytes + bytes(joystick_state) + rand_bytes
            packed_bytes += bytes([sum(packed_bytes) % 256])
            radio.write(packed_bytes)
        

if __name__ == '__main__':
    main()













