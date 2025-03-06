#line 1 "C:\\Users\\Titania\\Desktop\\isaac\\picolowlevel_x_vs\\PicoLowLevel\\README.md"
# PicoLowLevel

This is the code running on the board inside each module of Rese.Q.

## Module components

The components inside each modules are:

- Raspberry Pi Pico W
- CAN transceiver, with MCP2515 and TJA1050
- Two  DC motors
  - controlled by Pololu G2 24v13 drivers
  - rotary encoder with 48 pulses per motor rotation
  - 150:1 gearbox
- One to three Dynamixel AX-12A smart servo motors
- AMS AS5048B absolute encoder
- 64*128 OLED display with SH1106 driver

## Building

To build the project you need a working Arduino environment. This can be either the official Arduino IDE, VSCode with the [Arduino extension](https://github.com/microsoft/vscode-arduino), or even simply [arduino-cli](https://github.com/arduino/arduino-cli).

### Arduino-Pico

This project is based on the Arduino framework, and in particular uses the Raspberry Pi Pico available [here](https://github.com/earlephilhower/arduino-pico). The guide on how to install the core can be found in the repository's README.

### Libraries

Currently the only external library we are using is the `Adafruit SH110X` library, used to control the display. It can be found in Arduino's library manager.

### Build options

In the Arduino IDE the Raspberry Pi Pico W board should be selected, and the flash size should be set to `2MB (Sketch: 1MB, FS: 1MB)`, meaning that half of the microcontroller memory will be dedicated to the program itself, while the other half can be used for storing informations and performing over-the-air (OTA) upgrades.

#roba da installare attraverso i comandi