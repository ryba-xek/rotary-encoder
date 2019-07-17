#include "spi.h"

#include <avr/io.h>
#include <util/delay.h>

void SPI_MasterInit() {
    /* Set SS, MOSI and SCK output, all others input */
    DDRB = 1 << PIN_SS | 1 << PIN_MOSI | 1 << PIN_SCK;

    /* Enable SPI, SPI interrupt, MSB first, Master, Mode 1 (CPOL=0, CPHA=1), set clock rate fosc/2 */
    SPCR = 1 << SPE | 1 << MSTR | 1 << CPHA; // | 1 << SPIE
    SPSR = 1 << SPI2X;

    // set SS = high
    PORTB |= 1 << PIN_SS;
}

uint16_t SPI_MasterTransmit(uint16_t data) {
    union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } in, out;
    in.val = data;

    // transmit 2 bytes
    SPDR = in.msb;
    /*
    * The following NOP introduces a small delay that can prevent the wait
    * loop form iterating when running at the maximum speed. This gives
    * about 10% more speed, even if it seems counter-intuitive. At lower
    * speeds it is unnoticed.
    */
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ;
    out.msb = SPDR;

    SPDR = in.lsb;
    asm volatile("nop");
    while (!(SPSR & _BV(SPIF))) ;
    out.lsb = SPDR;

    return out.val;
}