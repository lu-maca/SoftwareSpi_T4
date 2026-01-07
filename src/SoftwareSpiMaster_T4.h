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

#ifndef SOFTWARESPIMASTER_T4_H
#define SOFTWARESPIMASTER_T4_H

#include "SoftwareSpi_T4.h"

class SoftwareSpiMaster
{
  const uint32_t delay_ns_;

 protected:
  // assert slave select
  inline void assert_ss(const byte ss_pin) const {
    digitalWriteFast(ss_pin, LOW);
  }
  // deassert slave select
  inline void deassert_ss(const byte ss_pin) const {
    digitalWriteFast(ss_pin, HIGH);
  }
  // rise clock
  inline void rise_sck() const {
    digitalWriteFast(sck_pin_, HIGH);
  }
  // drop clock
  inline void drop_sck() const {
    digitalWriteFast(sck_pin_, LOW);
  }

 public:
  // constructor
  SoftwareSpiMaster(const uint32_t frequency);
  // add device on pin `ss_pin`
  void add(byte ss_pin);
  // begin spi, specify common bus pins (sdi, sdo, sck)
  void begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin);
  // perform a full transaction, no check on parameters: `rx` and `tx` shall have same length!
  void transaction(const int ss_pin, const byte* tx, byte* rx, const int transaction_len) const;
  // perform a full transaction, no check on parameters: `rx` and `tx` shall have same length!
  void transaction(const int ss_pin, const byte* tx, volatile byte* rx, const int transaction_len) const;
};

#endif