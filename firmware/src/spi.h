//
// Created by xek on 16/07/2019.
//

#ifndef FIRMWARE_SPI_H
#define FIRMWARE_SPI_H

#include <stdint.h>
#include <avr/io.h>

#define PIN_MOSI  PINB3
#define PIN_MISO  PINB4
#define PIN_SCK   PINB5
#define PIN_SS    PINB2

// exports
void SPI_MasterInit();
uint16_t SPI_MasterTransmit(uint16_t data);

#define SPI_start_packet() PORTB &= ~(1 << PIN_SS)
#define SPI_end_packet() PORTB |= 1 << PIN_SS

#endif //FIRMWARE_SPI_H
