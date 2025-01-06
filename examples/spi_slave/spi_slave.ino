#include <SoftwareSpiSlave_T4.h>

constexpr int SS_PIN = 33;
constexpr int SCK_PIN = 34;
constexpr int SDI_PIN = 35;
constexpr int SDO_PIN = 36;

static SoftwareSpiSlave spiSlave{};

// forward declaratin of the ISR
static void slave_isr();

void setup() {
  // add a slave
  spiSlave.add(SS_PIN, slave_isr);
  // start the master, only after having added all the slaves
  spiSlave.begin(SDI_PIN, SDO_PIN, SCK_PIN);

  Serial.begin(115200);
}

void loop() {
  Serial.print("millis: "); Serial.println(millis());
  delay(1000);
}

static void slave_isr() {
  byte rx = 0;
  byte tx = 0;
  
  while (spiSlave.isActive(SS_PIN)) {
    spiSlave.byteTransaction(&rx, tx, SS_PIN);

    switch (rx) {
    case 0x10:
      tx = 0x20;
      break;
    case 0x20:
      tx = 0x40;
      break;
    default:
      tx = 0x00;
      break;
    }
  }
}
