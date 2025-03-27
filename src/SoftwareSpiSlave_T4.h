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

#ifndef SOFTWARESPISLAVE_T4_H
#define SOFTWARESPISLAVE_T4_H
#include <Arduino.h>

template <byte CPOL = 0, byte CPHA = 0>
class SoftwareSpiSlave
{
 private:
  byte sdi_pin_;
  byte sdo_pin_;
  byte sck_pin_;

 protected:
  // return `true` if we shall wait for reading the SDI
  inline const bool wait_for_read() const { 
    if constexpr(CPOL == CPHA) {
      return (digitalReadFast(sck_pin_) == LOW); 
    } else if constexpr(CPOL != CPHA) {
      return (digitalReadFast(sck_pin_) == HIGH); 
    }
  }
  // return `true` if we shall wait for writing on the SDO
  inline const bool wait_for_write() const { 
    if constexpr(CPOL == CPHA) {
      return (digitalReadFast(sck_pin_) == HIGH); 
    } else if constexpr(CPOL != CPHA) {
      return (digitalReadFast(sck_pin_) == LOW); 
    }
  }
  // read `bit_idx`-th bit on SDI line, store it in `rcv` `bit_idx`-th bit; note: bit endianness!
  inline void read_sdi(volatile byte* rcv, const byte bit_idx) const
  {
    bitWrite(*rcv, 7 - bit_idx, digitalReadFast(sdi_pin_));
  }
  // read `bit_idx`-th bit on SDI line, store it in `rcv` `bit_idx`-th bit; note: bit endianness!
  inline void read_sdi(byte* rcv, const byte bit_idx) const
  {
    bitWrite(*rcv, 7 - bit_idx, digitalReadFast(sdi_pin_));
  }
  // write `bit_idx`-th bit of `val` on SDO line; note: bit endianness!
  inline void write_sdo(const byte val, const byte bit_idx) const
  {
    digitalWriteFast(sdo_pin_, bitRead(val, 7 - bit_idx));
  }

 public:
  // constructor
  SoftwareSpiSlave();
  // add slave select on pin `ss_pin`, with handler `isr`
  void add(byte ss_pin, void (*isr)(void));
  // begin spi, call this after adding all slaves
  void begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin);
  // perform a transaction of a single byte
  void byteTransaction(byte* rx, const byte tx, const int ss_pin) const;
  // perform a transaction of a single byte
  void byteTransaction(volatile byte* rx, const byte tx, const int ss_pin) const;
  // check if specified slave select is asserted
  const bool isActive(const byte ss) const { return (digitalReadFast(ss) == LOW); }

};

#endif