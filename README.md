# SoftwareSpi_T4

SPI bit-banging for Teensy 4.

# Features
* supports master mode, with multiple slave devices
* supports slave mode, by emulating multiple slaves on a single SPI bus
* supports SPI mode 0 only (CPOL=0, CPHA=0)
* supports 8 bits messages only (no 16/32 bits)

# To do
* add support for other SPI modes
* add support for other transmission sizes