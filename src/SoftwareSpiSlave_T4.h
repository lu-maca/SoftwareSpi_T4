
#ifndef SOFTWARESPISLAVE_T4_H
#define SOFTWARESPISLAVE_T4_H
#include <Arduino.h>

enum class SpiMode : byte
{
  _0 = 0,
  _1 = 1,
  _2 = 2,
  _3 = 3
};

class SoftwareSpiSlave
{
private:
  const byte mode_;
  byte sdi_pin_;
  byte sdo_pin_;
  byte sck_pin_;
  bool mode02_;
  bool mode01_;
  bool mode13_;

protected:
  // return `true` if clock is HIGH
  inline const bool is_clock_high() const { return (digitalReadFast(sck_pin_) == HIGH); }
  // read `bit_idx`-th bit on SDI line, store it in `rcv` `bit_idx`-th bit; note: bit endianness!
  inline void read_sdi(volatile byte *rcv, const byte bit_idx) const
  {
    bitWrite(*rcv, 7 - bit_idx, digitalReadFast(sdi_pin_));
  }
  // read `bit_idx`-th bit on SDI line, store it in `rcv` `bit_idx`-th bit; note: bit endianness!
  inline void read_sdi(byte *rcv, const byte bit_idx) const
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
  SoftwareSpiSlave(const SpiMode mode);
  // add slave select on pin `ss_pin`, with handler `isr`
  void add(byte ss_pin, void (*isr)(void));
  // begin spi, call this after adding all slaves
  void begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin);
  // perform a transaction of a single byte
  void byteTransaction(byte *rx, const byte tx, const int ss_pin) const;
  // perform a transaction of a single byte
  void byteTransaction(volatile byte *rx, const byte tx, const int ss_pin) const;
  // check if specified slave select is asserted
  const bool isActive(const byte ss) const { return (digitalReadFast(ss) == LOW); }
};

#endif