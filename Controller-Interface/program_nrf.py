from common import find_serial_port
from time import sleep

def main():
    ser = find_serial_port()

    print("Configuring baud rate...")
    for i in (4800, 9600, 14400, 19200, 38400, 57600, 115200):
        ser.baudrate = i
        ser.write(b"AT+BAUD=7")
        sleep(0.1)

    channel = int(input("Enter team number: ")) * 3
    assert 0 <= channel <= 125
    freq = 2400 + channel

    print("Configuring channels...")

    ser.write(b"AT+FREQ=%.03fG" % (freq / 1000))
    sleep(0.1)
    ser.write(b"AT+TXA=0xF0,0xF0,0xF0,0xF0,0xE1")
    sleep(0.1)
    ser.write(b"AT+RXA=0xF0,0xF0,0xF0,0xF0,0xD2")
    sleep(0.1)
    ser.write(b"AT+CRC=16")
    sleep(0.1)
    ser.write(b"AT+RATE=2")

    print("Done!")
    
if __name__ == "__main__":
    main()
