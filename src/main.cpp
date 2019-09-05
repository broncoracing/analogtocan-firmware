/*
  Analog to CAN board firmware

  CAN data frame Layout:
  Bytes 0 - 3 uint16 encoded an1 Data
  Bytes 4 - 7 uint16 encoded an2 Data
*/

#include <mbed.h>

#define CAN_ID 1101
#define CAN_MSG_PERIOD_MS 100

CAN can0(PA_11, PA_12);

DigitalOut led(PB_0);

AnalogIn an1(PA_1);
AnalogIn an2(PA_2);

CANMessage msgOut;

uint16_t an1Data = 0;
uint16_t an2Data = 0;

int main() {

  msgOut.id = CAN_ID;
  msgOut.len = 8;
  msgOut.format = CANStandard;

  while (1) {

    an1Data = an1.read_u16();
    an2Data = an2.read_u16();

    memcpy(&an1Data, &msgOut.data[0], 4);
    memcpy(&an2Data, &msgOut.data[4], 4);

    if (can0.write(msgOut)) {
      led = !led;
    }
    wait_ms(CAN_MSG_PERIOD_MS);
  }
}