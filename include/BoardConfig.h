// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
#pragma once

// GPIO numbers, NOT physical ESP32 module pin numbers.
// Profile: JC4827W543 with NV3041A QSPI display and XPT2046 resistive touch.
namespace BoardConfig {
constexpr int Width = 480;
constexpr int Height = 272;
constexpr int DisplayCs = 45;
constexpr int DisplayClock = 47;
constexpr int DisplayD0 = 21;
constexpr int DisplayD1 = 48;
constexpr int DisplayD2 = 40;
constexpr int DisplayD3 = 39;
constexpr int Backlight = 1;
constexpr int BacklightChannel = 2;
constexpr int TouchClock = 12;
constexpr int TouchMiso = 13;
constexpr int TouchMosi = 11;
constexpr int TouchCs = 38;
constexpr int TouchIrq = 3;

// Starting values only. Run touch_calibration for your individual panel.
// Raw coordinates at the visible edges, with touch rotation fixed at zero.
constexpr int RawLeft = 200;
constexpr int RawRight = 3800;
constexpr int RawTop = 3800;
constexpr int RawBottom = 200;
constexpr bool SwapTouchAxes = false;
}

#ifndef STARTER_TOUCH_ENABLED
#define STARTER_TOUCH_ENABLED 1
#endif
