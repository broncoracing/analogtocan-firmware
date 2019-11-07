/*
  Analog to CAN board firmware

  CAN data frame Layout:
  Bytes 0 - 3 uint16 encoded an1 Data
  Bytes 4 - 7 uint16 encoded an2 Data
*/

#include <mbed.h>

#define CAN_ID 101
#define CAN_MSG_PERIOD_MS 100

CAN can(PA_11, PA_12);
SPI spi(PB_15, PB_14, PB_13, PB_12); // MOSI, MISO, SCK, SS

DigitalOut led(PB_0);

AnalogIn an1(PA_1);
AnalogIn an2(PA_2);

uint16_t an1Data = 0;
uint16_t an2Data = 0;

uint16_t digiPot1Config = 0b0000000000000000;
uint16_t digiPot2Config = 0b1000000000000000;

// int main() {

//   msgOut.id = CAN_ID;
//   msgOut.len = 8;
//   msgOut.format = CANStandard;

//   can.frequency(250000);

//   spi.frequency(1000000);
//   spi.format(9, 0);

//   spi.write(digiPot1Config);
//   spi.write(digiPot2Config);

//   while (1) {

//     an1Data = an1.read_u16();
//     an2Data = an2.read_u16();

//     memcpy(&an1Data, &msgOut.data[0], 4);
//     memcpy(&an2Data, &msgOut.data[4], 4);

//     if (can.write(msgOut)) {
//       led = !led;
//     }
//     wait_ms(CAN_MSG_PERIOD_MS);
//   }
// }

CANMessage msgOut;
float test = 0;

int main() {

  led = 0;

  msgOut.id = CAN_ID;
  msgOut.len = 8;
  msgOut.data[0] = 1;
  can.frequency(250000);

  wait(1);

  while (1) {

    test = an1.read();
    an2.read();

    if (can.write(msgOut)) {
      led = !led;
    }
    wait(.5);
  }
}
