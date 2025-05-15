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

void SoftwareSpiSlave::begin(const byte sdi_pin, const byte sdo_pin,
                             const byte sck_pin, const byte cpol,
                             const byte cpha) {
  sdi_pin_ = sdi_pin;
  sck_pin_ = sck_pin;
  sdo_pin_ = sdo_pin;
  cpol_ = cpol;
  cpha_ = cpha;

  pinMode(sdi_pin_, INPUT);
  pinMode(sdo_pin_, OUTPUT);
  pinMode(sck_pin_, INPUT);

  NVIC_SET_PRIORITY(IRQ_GPIO6789, 0);
}

void SoftwareSpiSlave::add(const byte ss_pin, void (*isr)(void)) {
  pinMode(ss_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ss_pin), isr, FALLING);
}

bool SoftwareSpiSlave::byteTransaction(byte *rx, const byte tx,
                                       const int ss_pin) const {
  static const bool edge1 = (cpha_ == 1);
  static const bool edge2 = (cpha_ == 0);

  // Transfer bits 0–6 with helper
  for (byte i = 0; i < 7; ++i) {
    if (!transfer_bit(rx, tx, i, edge1, edge2, ss_pin)) {
      return false;
    }
  }

  // Final bit 7 (no second edge)
  if (!isActive(ss_pin)) {
    return false;
  }
  write_sdo(tx, 7);
  if (!wait_for_edge(edge1, ss_pin)) {
    return false;
  }
  read_sdi(rx, 7);

  write_sdo(LOW, 0); // reset SDO
  return true;
}

bool SoftwareSpiSlave::byteTransaction(volatile byte *rx, const byte tx,
                                       const int ss_pin) const {
  static const bool edge1 = (cpha_ == 1);
  static const bool edge2 = (cpha_ == 0);

  // Transfer bits 0–6 with helper
  for (byte i = 0; i < 7; ++i) {
    if (!transfer_bit(rx, tx, i, edge1, edge2, ss_pin)) {
      return false;
    }
  }

  // Final bit 7 (no second edge)
  if (!isActive(ss_pin)) {
    return false;
  }
  write_sdo(tx, 7);
  if (!wait_for_edge(edge1, ss_pin)) {
    return false;
  }
  read_sdi(rx, 7);

  write_sdo(LOW, 0); // reset SDO
  return true;
}
