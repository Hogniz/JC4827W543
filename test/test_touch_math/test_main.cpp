// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
#include <unity.h>
#include "TouchMath.h"
void setUp() {}
void tearDown() {}
void test_corners_and_center() {
    const TouchMath::Calibration c{200, 3800, 3800, 200, false};
    auto p = TouchMath::map({200, 3800}, c, 480, 272);
    TEST_ASSERT_EQUAL_INT(0, p.x); TEST_ASSERT_EQUAL_INT(0, p.y);
    p = TouchMath::map({3800, 200}, c, 480, 272);
    TEST_ASSERT_EQUAL_INT(479, p.x); TEST_ASSERT_EQUAL_INT(271, p.y);
    p = TouchMath::map({2000, 2000}, c, 480, 272);
    TEST_ASSERT_INT_WITHIN(1, 240, p.x); TEST_ASSERT_INT_WITHIN(1, 136, p.y);
}
void test_clamp_both_directions() {
    TEST_ASSERT_EQUAL_INT(0, TouchMath::mapAxis(-100, 200, 3800, 480));
    TEST_ASSERT_EQUAL_INT(479, TouchMath::mapAxis(4095, 200, 3800, 480));
    TEST_ASSERT_EQUAL_INT(0, TouchMath::mapAxis(4095, 3800, 200, 272));
    TEST_ASSERT_EQUAL_INT(271, TouchMath::mapAxis(-100, 3800, 200, 272));
}
void test_swapped_axes() {
    const TouchMath::Calibration c{200, 3800, 3800, 200, true};
    const auto p = TouchMath::map({3800, 200}, c, 480, 272);
    TEST_ASSERT_EQUAL_INT(0, p.x); TEST_ASSERT_EQUAL_INT(0, p.y);
}
void test_invalid_ranges() {
    TEST_ASSERT_EQUAL_INT(0, TouchMath::mapAxis(100, 50, 50, 480));
    TEST_ASSERT_EQUAL_INT(0, TouchMath::mapAxis(100, 0, 200, 0));
    TEST_ASSERT_EQUAL_INT(0, TouchMath::mapAxis(100, 0, 200, 1));
    const TouchMath::Calibration bad{1, 1, 0, 4095, false};
    TEST_ASSERT_FALSE(bad.valid());
}
void test_calibration_from_inset_targets() {
    int first, last;
    TEST_ASSERT_TRUE(TouchMath::axisFromTargets(425, 3574, 30, 449, 480, first, last));
    TEST_ASSERT_INT_WITHIN(2, 200, first); TEST_ASSERT_INT_WITHIN(2, 3800, last);
    TEST_ASSERT_TRUE(TouchMath::axisFromTargets(3401, 599, 30, 241, 272, first, last));
    TEST_ASSERT_INT_WITHIN(2, 3800, first); TEST_ASSERT_INT_WITHIN(2, 200, last);
}
void test_calibration_rejects_same_spot_and_invalid_targets() {
    int first = 99, last = 99;
    TEST_ASSERT_FALSE(TouchMath::axisFromTargets(1000, 1100, 30, 449, 480, first, last));
    TEST_ASSERT_FALSE(TouchMath::axisFromTargets(200, 3800, 30, 30, 480, first, last));
    TEST_ASSERT_FALSE(TouchMath::axisFromTargets(200, 3800, 30, 480, 480, first, last));
    TEST_ASSERT_EQUAL_INT(99, first); TEST_ASSERT_EQUAL_INT(99, last);
}
void test_button_boundaries() {
    TEST_ASSERT_TRUE(TouchMath::contains({18, 152}, 18, 152, 140, 48));
    TEST_ASSERT_TRUE(TouchMath::contains({157, 199}, 18, 152, 140, 48));
    TEST_ASSERT_FALSE(TouchMath::contains({158, 199}, 18, 152, 140, 48));
    TEST_ASSERT_FALSE(TouchMath::contains({18, 200}, 18, 152, 140, 48));
    TEST_ASSERT_FALSE(TouchMath::contains({17, 152}, 18, 152, 140, 48));
}
int main() {
    UNITY_BEGIN();
    RUN_TEST(test_corners_and_center);
    RUN_TEST(test_clamp_both_directions);
    RUN_TEST(test_swapped_axes);
    RUN_TEST(test_invalid_ranges);
    RUN_TEST(test_calibration_from_inset_targets);
    RUN_TEST(test_calibration_rejects_same_spot_and_invalid_targets);
    RUN_TEST(test_button_boundaries);
    return UNITY_END();
}
