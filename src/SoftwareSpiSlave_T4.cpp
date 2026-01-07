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

#define BYTE_TRANSACTION                           \
  for (int bit_idx = 0; bit_idx < 8; bit_idx++)    \
  {                                                \
    if (mode02_)                                   \
    {                                              \
      write_sdo(tx, bit_idx);                      \
    }                                              \
    if (mode01_)                                   \
    {                                              \
      while (!is_clock_high() && isActive(ss_pin)) \
        ;                                          \
    }                                              \
    else                                           \
    {                                              \
      while (is_clock_high() && isActive(ss_pin))  \
        ;                                          \
    }                                              \
    if (mode02_)                                   \
    {                                              \
      read_sdi(rx, bit_idx);                       \
    }                                              \
    else                                           \
    {                                              \
      write_sdo(tx, bit_idx);                      \
    }                                              \
    if (mode01_)                                   \
    {                                              \
      while (is_clock_high() && isActive(ss_pin))  \
        ;                                          \
    }                                              \
    else                                           \
    {                                              \
      while (!is_clock_high() && isActive(ss_pin)) \
        ;                                          \
    }                                              \
    if (mode13_)                                         \
    {                                              \
      read_sdi(rx, bit_idx);                       \
    }                                              \
  }

SoftwareSpiSlave::SoftwareSpiSlave(const SpiMode mode)
{
  const byte mode_ = static_cast<byte>(mode);
  mode02_ = (mode_ == 0 || mode_ == 2);
  mode01_ = (mode_ == 0 || mode_ == 1);
  mode13_ = (mode_ == 1 || mode_ == 3);
}

void SoftwareSpiSlave::begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin)
{
  sdi_pin_ = sdi_pin;
  sck_pin_ = sck_pin;
  sdo_pin_ = sdo_pin;

  pinMode(sdi_pin_, INPUT);
  pinMode(sdo_pin_, OUTPUT);
  pinMode(sck_pin_, INPUT);
}

void SoftwareSpiSlave::add(const byte ss_pin, void (*isr)(void))
{
  pinMode(ss_pin, INPUT_PULLUP);
  // attach interrupt on falling chip select
  attachInterrupt(digitalPinToInterrupt(ss_pin), isr, FALLING);
}

void SoftwareSpiSlave::byteTransaction(byte *rx, const byte tx, const int ss_pin) const
{
  BYTE_TRANSACTION
}

void SoftwareSpiSlave::byteTransaction(volatile byte *rx, const byte tx, const int ss_pin) const
{
  BYTE_TRANSACTION
}