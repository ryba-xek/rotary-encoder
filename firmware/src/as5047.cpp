#include <SPI.h>
#include "as5047.h"

void initSensor() {
    SPI.begin();
}

SensorReply readSensor(uint16_t reg) {
    uint16_t sendBuf;
    uint16_t rcvBuf;
    SensorReply result = {};

    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));

    digitalWrite(SS, LOW);
    sendBuf = reg | uint16_t(bit(14)); //bit 14 — Read operation
    if (__builtin_parity(sendBuf)) sendBuf |= bit(15); // bit 15 - parityOk bit
    SPI.transfer16(sendBuf);
    digitalWrite(SS, HIGH);

    delayMicroseconds(10);

    digitalWrite(SS, LOW);
//    sendBuf = REG_NOP | bit(14); //bit 14 — Read operation
//    if (__builtin_parity(sendBuf)) sendBuf |= bit(15); // bit 15 - parityOk bit
    rcvBuf = SPI.transfer16(0x0000);
    digitalWrite(SS, HIGH);

    bool parityOk = __builtin_parity(rcvBuf & 0x7FFF) == bitRead(rcvBuf, 15);
    bool error = bitRead(rcvBuf, 14) == 1;
    result.success = parityOk && !error;
    result.data = rcvBuf & 0x3FFF;

    SPI.endTransaction();

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
