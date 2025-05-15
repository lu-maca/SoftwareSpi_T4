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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
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

class SoftwareSpiSlave {
private:
  byte sdi_pin_;
  byte sdo_pin_;
  byte sck_pin_;
  byte cpha_;
  byte cpol_;

protected:
  // Edge detection: wait for specified edge
  inline bool wait_for_edge(bool rising, byte ss_pin) const {
    bool prev = digitalReadFast(sck_pin_);
    while (true) {
      bool curr = digitalReadFast(sck_pin_);
      if (rising && !prev && curr)
        return true; // Rising edge
      if (!rising && prev && !curr)
        return true; // Falling edge
      if (!isActive(ss_pin))
        return false;
      prev = curr;
    }
  }

  inline const bool is_clock_high() const {
    return (digitalReadFast(sck_pin_) == HIGH);
  }

  // Read/write helpers
  inline void read_sdi(volatile byte *rcv, const byte bit_idx) const {
    bitWrite(*rcv, 7 - bit_idx, digitalReadFast(sdi_pin_));
  }

  inline void read_sdi(byte *rcv, const byte bit_idx) const {
    bitWrite(*rcv, 7 - bit_idx, digitalReadFast(sdi_pin_));
  }

  inline void write_sdo(const byte val, const byte bit_idx) const {
    digitalWriteFast(sdo_pin_, bitRead(val, 7 - bit_idx));
  }

  inline bool transfer_bit(byte *sdi_val, const byte tx, const byte bit_idx,
                           const bool edge1, const bool edge2,
                           const byte ss_pin) const {
    if (!isActive(ss_pin))
      return false;
    write_sdo(tx, bit_idx);
    if (!wait_for_edge(edge1, ss_pin))
      return false;
    read_sdi(sdi_val, bit_idx);
    if (!wait_for_edge(edge2, ss_pin))
      return false;
    return true;
  }

public:
  SoftwareSpiSlave();

  void add(byte ss_pin, void (*isr)(void));

  void begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin,
             const byte cpol, const byte cpha);

  bool byteTransaction(byte *rx, const byte tx, const int ss_pin) const;

  bool byteTransaction(volatile byte *rx, const byte tx,
                       const int ss_pin) const;

  const bool isActive(const byte ss) const {
    return (digitalReadFast(ss) == LOW);
  }
};

#endif
