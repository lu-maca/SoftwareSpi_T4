#include "SoftwareSpiMaster_T4.h"

SoftwareSpiMaster::SoftwareSpiMaster() {}

void SoftwareSpiMaster::begin(const byte sdi_pin, const byte sdo_pin, const byte sck_pin) {
  sdi_pin_ = sdi_pin;
  sck_pin_ = sck_pin;
  sdo_pin_ = sdo_pin;
  pinMode(sdi_pin_, INPUT);
  pinMode(sdo_pin_, OUTPUT);
  pinMode(sck_pin_, OUTPUT);
}

void SoftwareSpiMaster::add(byte ss_pin) {
  pinMode(ss_pin, OUTPUT);
  deassert_ss(ss_pin);
}


void SoftwareSpiMaster::transaction(const int ss_pin, const byte* tx, byte* rx, const int transaction_len) const {
  // start transaction
  assert_ss(ss_pin);
  // actual transaction
  for (int byte_idx = 0; byte_idx < transaction_len; byte_idx++) {
    for (int bit_idx = 0; bit_idx <8; bit_idx++) {
      write_sdo(tx[byte_idx], bit_idx);
      rise_sck();
      read_sdi(&rx[byte_idx], bit_idx);
      drop_sck();
    }
  }
  // stop transaction
  deassert_ss(ss_pin);
}

void SoftwareSpiMaster::transaction(const int ss_pin, const byte* tx, volatile byte* rx, const int transaction_len) const {
  // start transaction
  assert_ss(ss_pin);
  // actual transaction
  for (int byte_idx = 0; byte_idx < transaction_len; byte_idx++) {
    for (int bit_idx = 0; bit_idx <8; bit_idx++) {
      write_sdo(tx[byte_idx], bit_idx);
      rise_sck();
      read_sdi(&rx[byte_idx], bit_idx);
      drop_sck();
    }
  }
  // stop transaction
  deassert_ss(ss_pin);
}