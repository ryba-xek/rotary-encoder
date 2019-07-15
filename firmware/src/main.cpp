#include <Arduino.h>
#include "main.h"
#include "as5047.h"

/**
 * This code generates UVW hall effect sensor signals from absolute shaft angle for a 14 pole pairs motor.
 * Simple hysteresis is used to avoid switching noise.
 */

void setup() {
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_U, OUTPUT);
    pinMode(PIN_V, OUTPUT);
    pinMode(PIN_W, OUTPUT);

    // begin with UVW = 000 and orange led on
    PORTC = UVWVALUES[6] << 2 | 1;

    Serial.begin(115200);
    initSensor();
}

/**
 * Calculate new 14-pole pairs 6-state UVW sector from new angle value
 * This function should be called exactly once per new angle value received
 * @param angle 14-bit angle
 */
uint8_t calcSector(uint16_t angle) {
    static uint16_t currSector = 0;

    // Since we have 14 pole pairs and 14-bit number, we divide angle by 195 and get states 0 to 83,
    // with 4 extra numbers (16380, 16381, 16382, 16383). They are evenly spread across four equal angle sectors.
    if (angle >= 4095*1) angle--;
    if (angle >= 4095*2) angle--;
    if (angle >= 4095*3) angle--;
    if (angle >= 4095*4) angle--; //0..16379

    uint16_t newSector = angle / 195; //0..83
    if (newSector == currSector) return (uint8_t) currSector;

    // is it forward or backwards?
    uint16_t distFwd = (newSector < currSector) ? (newSector + 84 - currSector) : (newSector - currSector); //0..83
    uint16_t distBwd = (currSector < newSector) ? (currSector + 84 - newSector) : (currSector - newSector); //0..83
    if (distFwd > distBwd) {
        // new value should be >= start of that sector + hysteresis value
        if (angle > newSector * 195 + HYSTERESIS) {
            currSector = newSector;
        }
    } else {
        // new value should be <= end of that sector - hysteresis value
        if (angle < newSector * 195 + 194 - HYSTERESIS) {
            currSector = newSector;
        }
    }

    return (uint8_t) currSector;
}

void loop() {
    static uint16_t cycle = 0;
    static uint16_t angle;
    static uint8_t sector = 0; // 0..83
    static bool magL = false;
    static bool magH = false;
    static bool cordicError = false;
    static bool offsetCalibrated = false;
    static uint16_t magnitude = 0;

    SensorReply r;

    r = readSensorAngle();
    if (r.success) {
        // shift angle
//        int32_t rawAngle = (int32_t) r.data;
//        rawAngle += ZERO_SHIFT;
//        if (rawAngle < 0) rawAngle += MAX_ANGLE;
//        if (rawAngle > MAX_ANGLE) rawAngle -= MAX_ANGLE;
//        angle = uint16_t(rawAngle);
        angle = r.data;
        sector = calcSector(angle);
    }/* else {
        Serial.print("Raw angle read error");
    }*/

    cycle++;
    if (cycle >= 10000) {
        cycle = 0;
        r = readSensorDiag();
        if (r.success) {
            magL = (bool) bitRead(r.data, 11);
            magH = (bool) bitRead(r.data, 10);
            cordicError = (bool) bitRead(r.data, 9);
            offsetCalibrated = (bool) bitRead(r.data, 8);
//            uint8_t agc = (r.data & 0xFF);

//            r = readSensorMagnitude();
//            magnitude = r.success ? r.data : 0;

//            char angleBuf[10];
//            dtostrf(double(angle) * 360 / MAX_ANGLE, 3, 3, angleBuf);

//            char strBuf[200];
//            if (cordicError || !offsetCalibrated) {
//                sprintf(strBuf, "Cordic/offset error!");
//            } else {
//                sprintf(strBuf, "[%u], Sector: %u/83, corrected angle angle: %sº, MagL: %u, MagH: %u, Cof: %u, Lf: %u, AGC: %u, Magnitude: %u", angle, sector, angleBuf, magL, magH, cordicError, offsetCalibrated, agc, magnitude);
//            }
//            Serial.println(strBuf);
        }/* else {
            Serial.print("Diagnostic info read error");
        }*/
    }

    // write outputs
    if (!r.success || cordicError || !offsetCalibrated) {
        // the state of UVW is frozen to 000
        PORTC = (UVWVALUES[6] << 2) | 1;
    } else {
        PORTC = (UVWVALUES[sector % 6] << 2) | magL << 1 | magH; // magL and magH can't both be true
    }
}
