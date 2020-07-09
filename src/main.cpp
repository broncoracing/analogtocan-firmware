/**
 *  @brief Analog to CAN board firmware
 *
 *  CAN data frame Layout:
 *  Bytes 0 - 1 uint16 encoded an0 Data
 *  Bytes 2 - 3 uint16 encoded an1 Data
 *  CAN Packet size: 4 bytes
*/

#include <mbed.h>

#define CAN_ID 101
#define CAN_MSG_PERIOD_MS 100
#define CAN_BAUD 250000

#define SPI_FREQ 1000000

// ---------- Set up IOs ---------- //
CAN can(PA_11, PA_12);
SPI digiPot(PB_15, PB_14, PB_13); // MOSI, MISO, SCK, SS

DigitalOut led(PB_0);
DigitalOut slaveSel(PB_12);

AnalogIn an0(PA_1);
AnalogIn an1(PA_2);

// ---------- Globals ---------- //
uint16_t an0Data = 0;
uint16_t an1Data = 0;

CANMessage msgOut;
Timer timer;

// These values must be 9 bits
// [ID of POT (1 bit)] [resistance value (8 bits)]
uint16_t digiPot0Config = (uint16_t)0b011111111;
uint16_t digiPot1Config = (uint16_t)0b111111111;

// ---------- Prototypes ---------- //
void writeSensorData();

int main() {

  // use 11 bit CAN ID, with a packet size of 4 bytes and set ID.
  msgOut.format = CANStandard;
  msgOut.len = 4;
  msgOut.id = CAN_ID;

  can.frequency(CAN_BAUD);

  // 9 bit frames, SPI mode 0
  digiPot.format(9, 0);
  digiPot.frequency(SPI_FREQ);

  // perform one time setup of digipot
  digiPot.write(digiPot0Config);
  digiPot.write(digiPot1Config);

  // setup timer for periodic CAN frame
  timer.reset();
  timer.start();

  // superloop
  while (1) {
    if (timer.read_ms() >= CAN_MSG_PERIOD_MS) {
      writeSensorData();

      timer.reset();
      timer.start();
    }
  }
}

/// @brief Read both channels and send CAN frame
void writeSensorData() {
  // read ADC
  an0Data = an0.read_u16();
  an1Data = an1.read_u16();

  // build 4 byte packet
  memcpy(&an0Data, &msgOut.data[0], 2);
  memcpy(&an1Data, &msgOut.data[2], 2);

  // send message, upon success flip led state
  if (can.write(msgOut)) {
    led = !led;
  }
}