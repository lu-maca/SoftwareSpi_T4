#include <SoftwareSpiMaster_T4.h>

constexpr int SS_PIN = 33;
constexpr int SCK_PIN = 34;
constexpr int SDI_PIN = 35;
constexpr int SDO_PIN = 36;

static SoftwareSpiMaster spiMaster{};

constexpr int TRX_LEN = 4;
// transmission buffer
static const byte tx[TRX_LEN] = {0x10, 0x20, 0x30, 0x40};
// reception buffer
static byte rx[TRX_LEN] = {0x00};

void setup() {
  // add the slave
  spiMaster.add(SS_PIN);
  // start the master
  spiMaster.begin(SDI_PIN, SDO_PIN, SCK_PIN);

  Serial.begin(115200);
}

void loop() {
  Serial.print("millis: "); Serial.println(millis());
  delay(1000);

  spiMaster.transaction(SS_PIN, tx, rx, TRX_LEN);
  Serial.print("Received: ");
  for (int i = 0; i < TRX_LEN; i++) {
    Serial.print(rx[i], HEX);
  }
  Serial.println();
}
