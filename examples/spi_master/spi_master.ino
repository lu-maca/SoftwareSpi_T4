#include "SoftwareSpiMaster_T4.h"

constexpr int SS_PIN = 10;
constexpr int SCK_PIN = 13;
constexpr int SDI_PIN = 12;
constexpr int SDO_PIN = 11;

static SoftwareSpiMaster spiMaster{1000000};

constexpr int TRX_LEN = 8;
// transmission buffer
static const byte tx[TRX_LEN] = {0x10, 0x20, 0x30, 0x40, 0x50, 0xf0, 0x01, 0xFF};
// reception buffer
static byte rx[TRX_LEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {
  // add the slave
  spiMaster.add(SS_PIN);
  // start the master
  spiMaster.begin(SDI_PIN, SDO_PIN, SCK_PIN);

  Serial.begin(115200);
}

void loop() {
  delay(1000);

  spiMaster.transaction(SS_PIN, tx, rx, TRX_LEN);
  Serial.print("Received: ");
  for (int i = 0; i < TRX_LEN; i++) {
    Serial.print(rx[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}