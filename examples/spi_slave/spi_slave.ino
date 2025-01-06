#include "SpiSlave.h"

static void BMG250_isr() {
  byte byte_idx = 0;
  byte addr = 0;

  while (spiSlave->is_ss_asserted(SS_GYRO)) {
    if (!transaction) {
      transaction = true;
    }

    if (byte_idx == 0) {
      spiSlave->byte_transaction(&RxByte[byte_idx], 0, SS_GYRO);
      // store address, only if it the first byte ended
      addr = RxByte[byte_idx] & 0x7f;
    } else {
      // otherwise reply with the required values
      spiSlave->byte_transaction(&RxByte[byte_idx], Bmg250_Registers[addr + byte_idx - 1], SS_GYRO);
    }
    byte_idx++;
  }
}
