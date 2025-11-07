# Red-Robot-2023
A repository for all of the resouces for the 2023 Red Robot Hackathon.

## Software Setup

### Arduino IDE & Code Setup

1. Install [The Arduino IDE](https://www.arduino.cc/en/software).
2. Install [support for the Pi Pico](https://github.com/earlephilhower/arduino-pico#installation).
3. Download the repository and unzip it to a convenient place on your computer.
4. Open `Arduino-Starter.ino`
5. Select the Raspberry Pi Pico (NOT Pico W and NOT the Mbed version) in the dropdown on the top left. You may need to plug in the Pico first.
6. Once you have selected the Pico, go to the library manager, and search for and install "RF24".

### Controller Setup

#### Windows

Install [Python 3](https://www.python.org/downloads/) from the official website.

You will also need to install [drivers for the radio module](https://www.wch-ic.com/downloads/CH341SER_ZIP.html).

#### Mac

Install [Python 3](https://www.python.org/downloads/) from the official website.

The drivers for the radio module should be installed by default.

#### Linux (Ubuntu / Debian) 

Install Python 3, probably using a command like the following:

```
$ sudo apt install python3
```

If you're on Linux, you may need to uninstall the Braille TTY to make the radio work properly.

```
$ sudo apt remove brltty
```

Then, you will need to add your user to the dialout group to use the radio.

```
$ sudo useradd yourusernamehere dialout
```

Log out and log in again to make the group change take effect.

## Programming the Radio
Before you can control your robot you will need to program your radio. Plug
the radio into the adapter board like this:

![](radio.png)

Run either `program_radio_mac.sh` or `program_radio_windows.sh`, depending on
your operating system. This will prompt for your team number (this is the
number on the side of your kit)

## Deploying Code

Steps 1-3 may not be necessary depending on your computer.
If the Arduino IDE tells you it can't find the Pico, try doing them.

1. Unplug the Pico from battery power and from the USB port.
  (It may be easiser to unplug the USB cable from your computer, as the Pico's USB port can be stiff).
2. Hold down the small button on the Pico.
3. While still holding down the button, plug the USB port back in.
4. Press the "Upload" button (arrow) in the top left corner.

The Arduino IDE's serial monitor can be somewhat unreliable,
so if you'd like to see output from your Pico,
you should probably [install CoolTerm](https://freeware.the-meiers.org).
Once in CoolTerm, simply select the Pico in the bottom left and set the Baud Rate to 115200, then click "connect."
Be careful not to accidentally select the radio module instead.


## Controlling The Robot

1. Flip the controller over and make sure the switch on the back is set to "D".
2. Plug the controller into your computer.
3. Plug the radio module into your computer.
5. Depending on your operating system, open `run_mac.sh` or `run_windows.bat`.
