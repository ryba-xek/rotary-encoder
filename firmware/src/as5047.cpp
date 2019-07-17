#include <util/delay.h>
#include "spi.h"
#include "as5047.h"

void initSensor() {
    SPI_MasterInit();
}

SensorReply readSensor(uint16_t reg) {
    uint16_t sendBuf;
    uint16_t rcvBuf;
    SensorReply result = {};

    SPI_start_packet();
    sendBuf = reg | 1 << 14; //bit 14 — Read operation
    if (__builtin_parity(sendBuf)) sendBuf |= 1 << 15; // bit 15 - parity bit
    rcvBuf = SPI_MasterTransmit(sendBuf);
    SPI_end_packet();

    for (uint8_t i = 0; i < 5; i++) asm volatile("nop"); //350 ns delay
//    _delay_us(1);

    SPI_start_packet();
    rcvBuf = SPI_MasterTransmit(0x0000);
    SPI_end_packet();

    bool parityOk = __builtin_parity(rcvBuf & 0x7FFF) == ((rcvBuf >> 15) & 1);
    bool error = ((rcvBuf >> 14) & 1) == 1;
    result.success = parityOk && !error;
    result.data = rcvBuf & 0x3FFF;

//    SPI_end_packet();

    return result;
}

SensorReply readSensorAngle() {
    return readSensor(REG_ANGLECOM);
}

SensorReply readSensorDiag() {
    return readSensor(REG_DIAAGC);
}

SensorReply readSensorMagnitude() {
    return readSensor(REG_MAG);
}
