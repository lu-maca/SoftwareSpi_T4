# SoftwareSpi_T4

SPI bit-banging for Teensy 4.

This library comes from [this](https://forum.pjrc.com/index.php?threads/spi-slave-on-teensy-4-1.76286/) thread. I needed to emulate the behavior of a sensor, that behaves as a SPI slave. The sensor shall reply dynamically to any request of a microcontroller, and RT1060 is not capable of this. In particular, you have to decide the contents of the reply *before* the message from the master arrives, and this does not suite with my needs. That's why I implemented a software SPI, that supports both master and slave modes.

In slave mode, I tried running it up to 4 Mbps and still works fine.
Master mode is not tested yet, but I'm quite confident it works. 

# Features
* supports master mode, with multiple slave devices (supports SPI mode 0 only)
* supports slave mode, by emulating multiple slaves on a single SPI bus (supports all SPI modes)
* supports 8 bits messages only (no 16/32 bits)

# How to install
Download the repository and follow the instructions [here](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries/#importing-a-zip-library).

# To do
* add support for other SPI modes for SPI master
* add support for other transmission sizes

# SPI Logic
|  0    |   1   |   2   |   3   |
|-------|-------|-------|-------|
| write |       | write |       |
| clk H | clk H | clk L | clk L |
| read  | write | read  | write |
| clk L | clk L | clk H | clk H |
|       | read  |       | read  |