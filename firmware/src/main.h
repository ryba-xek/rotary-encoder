//
// Created by xek on 05/07/2019.
//

#ifndef ROTARY_ENCODER_MAIN_H
#define ROTARY_ENCODER_MAIN_H

#include <stdint.h>

#define PIN_LED1  PIN_A0
#define PIN_LED2  PIN_A1
#define PIN_W PIN_A2
#define PIN_V PIN_A3
#define PIN_U PIN_A4

#define HYSTERESIS 2 // should be significantly lower than 195/2
#define ZERO_SHIFT -1450 // should be significantly lower than 195/2

uint8_t UVWVALUES[7] = {
        0b100,
        0b101,
        0b001,
        0b011,
        0b010,
        0b110,
        0b000, //error
};

#endif //ROTARY_ENCODER_MAIN_H
