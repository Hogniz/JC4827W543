// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
#pragma once
#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "BoardConfig.h"
#include "TouchMath.h"

namespace Board {
constexpr uint16_t Background = 0x0843;
constexpr uint16_t Panel = 0x1947;
constexpr uint16_t White = 0xFFFF;
constexpr uint16_t Muted = 0xA534;
constexpr uint16_t Accent = 0x05D6;
struct Touch {
    bool down = false;
    bool pressed = false;
    TouchMath::Point raw{0, 0};
    TouchMath::Point pixel{0, 0};
    int pressure = 0;
};
bool begin();
Arduino_Canvas& screen();
Touch readTouch();
void brightness(uint8_t value);
void diagnostics();
void label(int x, int y, const char* text, uint16_t color = White, uint8_t size = 2);
void button(int x, int y, int width, int height, const char* text);
void fatal(const char* message);
}
