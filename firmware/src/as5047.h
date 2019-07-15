//
// Created by xek on 05/07/2019.
//

#ifndef FIRMWARE_AS5047_H
#define FIRMWARE_AS5047_H

#include <stdint.h>

#define SPI_SPEED 10000000
#define MAX_ANGLE 0x3FFF

// AS5047D registers
#define REG_NOP      0x0000 // No operation
#define REG_ERRFL    0x0001 // Error register
#define REG_PROG     0x0003 // Programming register
#define REG_DIAAGC   0x3FFC // Diagnostic and AGC
#define REG_MAG      0x3FFD // CORDIC magnitude
#define REG_ANGLEUNC 0x3FFE // Measured angle without dynamic angle error compensation
#define REG_ANGLECOM 0x3FFF // Measured angle with dynamic angle error compensation

struct SensorReply {
    bool success;
    uint16_t data;
};

void initSensor();
SensorReply readSensorAngle();
SensorReply readSensorDiag();
SensorReply readSensorMagnitude();

#endif //FIRMWARE_AS5047_H
