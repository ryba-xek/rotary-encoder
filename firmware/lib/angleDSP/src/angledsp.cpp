#include "angledsp.h"

/**
 * Calculate new 14-pole pairs 6-state UVW sector from new angle value
 * This function should be called exactly once per new angle value received
 * @param angle *normalized* 14-bit angle 0..16379
 */
uint8_t calcSector(uint8_t currSector, uint16_t angle, uint16_t hyst) {
    uint8_t newSector = angle / 195; //0..83

    if (newSector == currSector) {
        return currSector;
    }

    // is it forward or backwards?
    uint8_t distFwd = calcDistFwd(currSector, newSector); //0..83
    uint8_t distBwd = calcDistBwd(currSector, newSector); //0..83
    // TODO: try algo with no hysteresis when dist > 1
    if (distFwd <= distBwd) {
        // if new value is before hysteresis boundary of new sector choose previous one
        if (angle >= newSector * 195 + hyst) {
            return newSector;
        } else {
            return (newSector == 0) ? uint8_t(83) : newSector-uint8_t(1);
        }
    } else {
        // if new value is before hysteresis boundary of new sector choose previous one (according to reverse rotation)
        if (angle <= newSector * 195 + 194 - hyst) {
            return newSector;
        } else {
            return (newSector == 83) ? uint8_t(0) : newSector+uint8_t(1);
        }
    }
}
