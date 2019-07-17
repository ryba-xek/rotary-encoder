//
// Created by xek on 16/07/2019.
//

#ifndef FIRMWARE_ANGLE_H
#define FIRMWARE_ANGLE_H

#include <stdint.h>

uint8_t calcSector(uint8_t currSector, uint16_t angle, uint16_t hyst);

// Since we have 14 pole pairs and 14-bit number, we divide angle by 195 and get states 0 to 83,
// with 4 extra numbers (16380, 16381, 16382, 16383). They are evenly spread across four equal angle sectors.
inline uint16_t normalizeAngle(uint16_t angle) {
    if      (angle == 16383) angle -= 4; // 4/4 12288..16383 -> 12285..16379
    else if (angle >= 12287) angle -= 3; // 3/4 8192..12287 -> 8190..12284
    else if (angle >= 8191)  angle -= 2; // 2/4 4096..8191 -> 4095..8190
    else if (angle >= 4095)  angle -= 1; // 1/4 0..4095 -> 0..4094
    return angle;
}

inline uint8_t calcDistFwd(uint8_t currSector, uint8_t newSector) {
    return newSector < currSector ? (newSector + uint8_t(84) - currSector) : (newSector - currSector);
};

inline uint8_t calcDistBwd(uint8_t currSector, uint8_t newSector) {
    return newSector > currSector ? (currSector + uint8_t(84) - newSector) : (currSector - newSector);
};

#endif //FIRMWARE_ANGLE_H
