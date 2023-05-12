# Easy CH32V003
This repository is intended to serve as quick examples on how to do things with ch32v003
The idea is just to handle single .c files with everything you need on them to just paste on a blank MounRiver studio and make it run. Also including some quick reference files like pinouts.


# CH32V003J4M6 pinout

![CH32V003J4M6 pinout](/images/ch32v003j4m6-pinout.png "CH32V003J4M6 pinout")

You can use an crystal on the PD6 and PA2 pins, but thats optional.

![CH32V003J4M6 diagram](/images/ch32v003j4m6-diagram.png "CH32V003J4M6 diagram")


# CH32V00F4P6 pinout

![CH32V00F4P6 pinout](/images/ch32v003f4p6-pinout.png "CH32V00F4P6 pinout")

You can use an crystal on the PD6 and PA2 pins, but thats optional.

![CH32V00F4P6 diagram](/images/ch32v003f4p6-diagram.png "CH32V00F4P6 diagram")

# Connecting elink

![CH32V00F4P6 elink](/images/ch32v003f4p6-programming.png "CH32V00F4P6 elink")

You only need the GND, VCC and the SWIO pin connected to flash in to the MCU. The serial port is optional