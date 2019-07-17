/**
 * This code generates UVW hall effect sensor signals from absolute shaft angle for a 14 pole pairs motor.
 * Simple hysteresis is used to avoid switching noise.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <angledsp.h>
#include "main.h"
#include "as5047.h"


void USART_Init(void){
    // Set baud rate
    UBRR0 = BAUD_PRESCALE;

    // 115200,8,N,1
    UCSR0A = 1 << U2X0;
    UCSR0C = 1 << UCSZ00 | 1 << UCSZ01;
    UCSR0B = 1 << TXEN0;

}

void USART_SendByte(uint8_t u8Data){

    // Wait until last byte has been transmitted
    while((UCSR0A & (1<<UDRE0)) == 0) ;

    // Transmit data
    UDR0 = u8Data;
}

void USART_send_string(char s[])
{
    int i =0;

    while (s[i] != 0x00)
    {
        USART_SendByte((uint8_t) s[i]);
        i++;
    }
}

int main(void)
{
    uint16_t cycle = 0;
    uint8_t sector = 0;
    SensorReply r;

    USART_Init();
    initSensor();

    // output pins
    DDRC = 1 << PIN_LED1 | 1 << PIN_LED2 | 1 << PIN_W | 1 << PIN_V | 1 << PIN_U;
    DDRD = 1 << PIND1;

    #pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1)
    {
//        cycle++;

        r = readSensorAngle();
        if (r.success) {
            sector = calcSector(sector, normalizeAngle(r.data), HYSTERESIS);
        }

//        if (cycle >= 5000) {
//            char strBuf[256];
//            sprintf(strBuf, "[%u] %u - %u\n", sector, r.data, r.success);
//            USART_send_string(strBuf);
//            cycle = 0;
//        }

        // write outputs
//    if (!r.success || cordicError || !offsetCalibrated) {
//        // the state of UVW is frozen to 000
//        PORTC = (UVWVALUES[6] << 2) | 1;
//    } else {
        PORTC = (UVWVALUES[sector % 6]);// | magL << 1 | magH; // magL and magH can't both be true
//    }
//        _delay_us(1);
//        PORTC = ~PORTC;
//        _delay_us(1);
//        PORTC = ~PORTC;
    }

    return 0;
}