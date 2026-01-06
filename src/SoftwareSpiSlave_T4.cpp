
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
    if (_)                                         \
    {                                              \
      read_sdi(rx, bit_idx);                       \
    }                                              \
  }

SoftwareSpiSlave::SoftwareSpiSlave(const SpiMode mode)
{
  const byte mode = static_cast<byte>(mode);
  mode02_ = (mode == 0 || mode == 2);
  mode01_ = (mode == 0 || mode == 1);
  mode13_ = (mode == 1 || mode == 3);
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