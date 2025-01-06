# SoftwareSpi_T4

SPI bit-banging for Teensy 4.

# Features
* supports master mode, with multiple slave devices
* supports slave mode, by emulating multiple slaves on a single SPI bus
* supports SPI mode 0 only (CPOL=0, CPHA=0)
* supports 8 bits messages only (no 16/32 bits)

# How to install
Download the repository and follow the instructions [here](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#importing-a-zip-library).

# To do
* add support for other SPI modes
* add support for other transmission sizes