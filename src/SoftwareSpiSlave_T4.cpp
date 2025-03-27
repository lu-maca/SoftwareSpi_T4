/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Adafruit Industries
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "SoftwareSpiSlave_T4.h"

SoftwareSpiSlave::SoftwareSpiSlave() {}

void SoftwareSpiSlave::begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin) {
  static_assert(CPOL < 2, "CPOL shall be 0 or 1");
  static_assert(CPHA < 2, "CPHA shall be 0 or 1");
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
    // wait to be ready to read and checking for ss to be asserted too
    while (wait_for_read() && isActive(ss_pin) )
      ;
    // when clock goes HIGH, read the bit on SDI
    read_sdi(rx, bit_idx);
    // wait to be ready to write
    while (wait_for_write())
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
     // wait to be ready to read and checking for ss to be asserted too
     while (wait_for_read() && isActive(ss_pin) )
      ;
    // when clock goes HIGH, read the bit on SDI
    read_sdi(rx, bit_idx);
    // wait to be ready to write
    while (wait_for_write())
      ;
  }
  // this is needed to reset the SDO line after the last bit of the first byte. If
  // not, the last bit of the first byte is set as first bit of the second byte, since
  // the master samples at rising clock and rising clock is checked right after this!
  write_sdo(0, LOW);
}