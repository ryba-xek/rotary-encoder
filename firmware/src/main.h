//
// Created by xek on 05/07/2019.
//

#ifndef ROTARY_ENCODER_MAIN_H
#define ROTARY_ENCODER_MAIN_H

#include <stdint.h>
#include <avr/io.h>

#define PIN_LED1  PINC0
#define PIN_LED2  PINC1
#define PIN_W     PINC2
#define PIN_V     PINC3
#define PIN_U     PINC4

#define HYSTERESIS 1 // measured noise is around +/- 2.5, so 1—3 is recommended value
#define ZERO_SHIFT -1450

#define USART_BAUDRATE 115200
#define BAUD_PRESCALE ((F_CPU / 4 / USART_BAUDRATE - 1) / 2)

uint8_t UVWVALUES[7] = {
        0b10000,
        0b10100,
        0b00100,
        0b01100,
        0b01000,
        0b11000,
        0b00000, //error
};

uint8_t calcSector(uint8_t currSector, uint16_t angle);

#endif //ROTARY_ENCODER_MAIN_H
