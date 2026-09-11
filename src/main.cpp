// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
// Interactive demo: counter, color selection, brightness and touch coordinates.
#include "Board.h"

namespace {
int counter = 0;
int colorIndex = 0;
int brightnessIndex = 2;
constexpr uint8_t levels[] = {40, 120, 200, 255};
constexpr uint16_t colors[] = {Board::Accent, 0xFC60, 0xF81F};
uint32_t lastPaint = 0;
uint32_t lastDiagnostics = 0;
bool dirty = true;
Board::Touch lastTouch;

void render() {
    auto& gfx = Board::screen();
    gfx.fillScreen(Board::Background);
    Board::label(18, 16, "JC4827W543", Board::White, 3);
    Board::label(18, 48, "NV3041A + XPT2046 / Arduino", Board::Muted, 1);
    gfx.fillRoundRect(18, 70, 444, 68, 10, Board::Panel);
    char line[64];
    snprintf(line, sizeof(line), "COUNT %d", counter);
    Board::label(34, 88, line, colors[colorIndex], 3);
    Board::button(18, 152, 140, 48, "+1");
    Board::button(170, 152, 140, 48, "COLOR");
    Board::button(322, 152, 140, 48, "LIGHT");
    snprintf(line, sizeof(line), "Touch x:%3d y:%3d  light:%3u", lastTouch.pixel.x,
             lastTouch.pixel.y, levels[brightnessIndex]);
    Board::label(18, 216, line, Board::Muted, 1);
    Board::label(18, 244, "Tap a button. Serial: i = info, r = reset count.", Board::White, 1);
    gfx.flush();
}
}

void setup() {
    if (!Board::begin()) Board::fatal("Initialization failed. See docs/troubleshooting.md.");
    render();
}

void loop() {
    const auto touch = Board::readTouch();
    if (touch.down) { lastTouch = touch; dirty = true; }
    if (touch.pressed) {
        if (TouchMath::contains(touch.pixel, 18, 152, 140, 48)) counter = (counter + 1) % 10000;
        if (TouchMath::contains(touch.pixel, 170, 152, 140, 48)) colorIndex = (colorIndex + 1) % 3;
        if (TouchMath::contains(touch.pixel, 322, 152, 140, 48)) {
            brightnessIndex = (brightnessIndex + 1) % 4;
            Board::brightness(levels[brightnessIndex]);
        }
    }
    while (Serial.available()) {
        const char command = Serial.read();
        if (command == 'i') Board::diagnostics();
        if (command == 'r') { counter = 0; dirty = true; }
    }
    const uint32_t now = millis();
    if (dirty && now - lastPaint >= 50) { render(); lastPaint = now; dirty = false; }
    if (now - lastDiagnostics >= 10000) {
        Serial.printf("Uptime: %lu s | free heap: %u\n", (unsigned long)(now / 1000), ESP.getFreeHeap());
        lastDiagnostics = now;
    }
    delay(10);
}
