/*
 Copyright (c) 2014-present PlatformIO <contact@platformio.org>
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
    http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
**/

#include <stdint.h>
#include <angledsp.h>
#include <unity.h>

void test_normalizeAngle() {
    // 1/4 0..4095 -> 0..4094
    TEST_ASSERT_EQUAL(0, normalizeAngle(0));
    TEST_ASSERT_EQUAL(1, normalizeAngle(1));
    TEST_ASSERT_EQUAL(2, normalizeAngle(2));
    TEST_ASSERT_EQUAL(4094,  normalizeAngle(4094));
    TEST_ASSERT_EQUAL(4094,  normalizeAngle(4095));

    // 2/4 4096..8191 -> 4095..8189
    TEST_ASSERT_EQUAL(4095,  normalizeAngle(4096));
    TEST_ASSERT_EQUAL(8189,  normalizeAngle(8190));
    TEST_ASSERT_EQUAL(8189,  normalizeAngle(8191));

    // 3/4 8192..12287 -> 8190..12284
    TEST_ASSERT_EQUAL(8190,  normalizeAngle(8192));
    TEST_ASSERT_EQUAL(8191,  normalizeAngle(8193));
    TEST_ASSERT_EQUAL(12282, normalizeAngle(12284));
    TEST_ASSERT_EQUAL(12283, normalizeAngle(12285));
    TEST_ASSERT_EQUAL(12284, normalizeAngle(12286));
    TEST_ASSERT_EQUAL(12284, normalizeAngle(12287));

    // 4/4 12288..16383 -> 12285..16379
    TEST_ASSERT_EQUAL(12285, normalizeAngle(12288));
    TEST_ASSERT_EQUAL(16376, normalizeAngle(16379));
    TEST_ASSERT_EQUAL(16377, normalizeAngle(16380));
    TEST_ASSERT_EQUAL(16378, normalizeAngle(16381));
    TEST_ASSERT_EQUAL(16379, normalizeAngle(16382));
    TEST_ASSERT_EQUAL(16379, normalizeAngle(16383));
}

void test_calcDist() {
    // fwd
    TEST_ASSERT_EQUAL(0, calcDistFwd(0, 0));
    TEST_ASSERT_EQUAL(0, calcDistFwd(83, 83));
    TEST_ASSERT_EQUAL(1, calcDistFwd(0, 1));
    TEST_ASSERT_EQUAL(2, calcDistFwd(0, 2));
    TEST_ASSERT_EQUAL(1, calcDistFwd(83, 0));
    TEST_ASSERT_EQUAL(42, calcDistFwd(0, 42));
    TEST_ASSERT_EQUAL(42, calcDistFwd(42, 0));
    TEST_ASSERT_EQUAL(83, calcDistFwd(2, 1));
    TEST_ASSERT_EQUAL(82, calcDistFwd(3, 1));
    TEST_ASSERT_EQUAL(83, calcDistFwd(0, 83));

    //bwd
    TEST_ASSERT_EQUAL(0, calcDistBwd(0, 0));
    TEST_ASSERT_EQUAL(0, calcDistBwd(83, 83));
    TEST_ASSERT_EQUAL(83, calcDistBwd(0, 1));
    TEST_ASSERT_EQUAL(82, calcDistBwd(0, 2));
    TEST_ASSERT_EQUAL(83, calcDistBwd(83, 0));
    TEST_ASSERT_EQUAL(42, calcDistBwd(0, 42));
    TEST_ASSERT_EQUAL(42, calcDistBwd(42, 0));
    TEST_ASSERT_EQUAL(1, calcDistBwd(2, 1));
    TEST_ASSERT_EQUAL(2, calcDistBwd(3, 1));
    TEST_ASSERT_EQUAL(1, calcDistBwd(0, 83));
}

void test_calcSector() {
    // no hysteresis
    uint16_t hyst = 0;
    TEST_ASSERT_EQUAL(83, calcSector(83, 16379, hyst));
    TEST_ASSERT_EQUAL(83, calcSector(0, 16379, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(0, 0, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(83, 0, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(83, 194, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(83, 195, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(0, 195, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(1, 195, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(1, 194, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(0, 389, hyst));
    TEST_ASSERT_EQUAL(2, calcSector(0, 390, hyst));

    // skipping
    TEST_ASSERT_EQUAL(0, calcSector(83, 193, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(83, 194, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(83, 195, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(83, 196, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(1, 1, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(1, 0, hyst));
    TEST_ASSERT_EQUAL(83, calcSector(1, 16379, hyst));
    TEST_ASSERT_EQUAL(83, calcSector(1, 16378, hyst));
    TEST_ASSERT_EQUAL(2, calcSector(0, 583, hyst));
    TEST_ASSERT_EQUAL(2, calcSector(0, 584, hyst));
    TEST_ASSERT_EQUAL(3, calcSector(0, 585, hyst));

    // hysteresis = 1
    hyst = 1;
    TEST_ASSERT_EQUAL(83, calcSector(83, 16377, hyst));
    TEST_ASSERT_EQUAL(83, calcSector(83, 16378, hyst));
    TEST_ASSERT_EQUAL(83, calcSector(83, 16379, hyst));
    TEST_ASSERT_EQUAL(83, calcSector(83, 0, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(83, 1, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(0, 0, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(0, 16379, hyst));
    TEST_ASSERT_EQUAL(83, calcSector(0, 16378, hyst));

    // skipping
    TEST_ASSERT_EQUAL(0, calcSector(83, 194, hyst));
    TEST_ASSERT_EQUAL(0, calcSector(83, 195, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(83, 196, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(0, 389, hyst));
    TEST_ASSERT_EQUAL(1, calcSector(0, 390, hyst));
    TEST_ASSERT_EQUAL(2, calcSector(0, 391, hyst));
    TEST_ASSERT_EQUAL(2, calcSector(0, 583, hyst));
    TEST_ASSERT_EQUAL(2, calcSector(0, 584, hyst));
    TEST_ASSERT_EQUAL(2, calcSector(0, 585, hyst));
    TEST_ASSERT_EQUAL(3, calcSector(0, 586, hyst));
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_normalizeAngle);
    RUN_TEST(test_calcDist);
    RUN_TEST(test_calcSector);
    UNITY_END();

    return 0;
}