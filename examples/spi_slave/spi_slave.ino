#include <SPI.h>
#include "SoftwareSpiSlave_T4.h"

constexpr int SS_PIN = 36;
constexpr int SCK_PIN = 33;
constexpr int SDI_PIN = 35;
constexpr int SDO_PIN = 34;

static SoftwareSpiSlave spiSlave{SpiMode::_0};

// forward declaratin of the ISR
static void slave_isr();

const byte BUF_LEN = 8;
uint8_t slave_tx_buf[BUF_LEN] = {
    0xF0, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

uint8_t rx_buf[BUF_LEN];

void setup()
{
    Serial.begin(115200);
    Serial.println("Reboot");
    // add a slave
    spiSlave.add(SS_PIN, slave_isr);
    // start the master, only after having added all the slaves
    spiSlave.begin(SDI_PIN, SDO_PIN, SCK_PIN);
    
}

void loop()
{
    delay(1000);
}

static void slave_isr() {
  int i = 0;
  while (spiSlave.isActive(SS_PIN)) {
    spiSlave.byteTransaction(&rx_buf[i], slave_tx_buf[i], SS_PIN);
    i++;
  }

  Serial.print("Received: ");
  for (int j = 0; j < 8; j++) {
    Serial.print(" ");
    Serial.print(rx_buf[j], HEX);
  }
  Serial.println();
}
