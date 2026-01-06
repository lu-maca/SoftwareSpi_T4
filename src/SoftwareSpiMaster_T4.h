
#ifndef SOFTWARESPIMASTER_T4_H
#define SOFTWARESPIMASTER_T4_H
#include <Arduino.h>

class SoftwareSpiMaster
{
 private:
  byte sdi_pin_;
  byte sdo_pin_;
  byte sck_pin_;
  const uint32_t delay_ns_;

 protected:
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