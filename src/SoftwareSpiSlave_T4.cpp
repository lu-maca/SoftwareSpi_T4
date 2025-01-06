
#include "SoftwareSpiSlave_T4.h"

SoftwareSpiSlave::SoftwareSpiSlave() {}

void SoftwareSpiSlave::begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin) {
  sdi_pin_ = sdi_pin;
  sck_pin_ = sck_pin;
  sdo_pin_ = sdo_pin;
  pinMode(sdi_pin_, INPUT);
  pinMode(sdo_pin_, OUTPUT);
  pinMode(sck_pin_, INPUT);
  // set interrupts priority on all pins
  NVIC_SET_PRIORITY(IRQ_GPIO6789, 0);
}

void SoftwareSpiSlave::add(const byte ss_pin, void (*isr)(void)) {
  pinMode(ss_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ss_pin), isr, FALLING);
}

void SoftwareSpiSlave::byteTransaction(byte* rx, const byte tx, const int ss_pin) const {
  for (int bit_idx = 0; bit_idx < 8; bit_idx++) {
    write_sdo(tx, bit_idx);
    // wait for clock to become HIGH and checking for ss to be asserted too
    while (!is_clock_high() && isActive(ss_pin) )
      ;
    // when clock goes HIGH, read the bit on SDI
    read_sdi(rx, bit_idx);
    // wait for clock to become LOW again
    while (is_clock_high())
      ;
  }
  // this is needed to reset the SDO line after the last bit of the first byte. If
  // not, the last bit of the first byte is set as first bit of the second byte, since
  // the master samples at rising clock and rising clock is checked right after this!
  write_sdo(0, LOW);
}

void SoftwareSpiSlave::byteTransaction(volatile byte* rx, const byte tx, const int ss_pin) const {
  for (int bit_idx = 0; bit_idx < 8; bit_idx++) {
    write_sdo(tx, bit_idx);
    // wait for clock to become HIGH and checking for ss to be asserted too
    while (!is_clock_high() && isActive(ss_pin))
      ;
    // when clock goes HIGH, read the bit on SDI
    read_sdi(rx, bit_idx);
    // wait for clock to become LOW again
    while (is_clock_high())
      ;
  }
  // this is needed to reset the SDO line after the last bit of the first byte. If
  // not, the last bit of the first byte is set as first bit of the second byte, since
  // the master samples at rising clock and rising clock is checked right after this!
  write_sdo(0, LOW);
}