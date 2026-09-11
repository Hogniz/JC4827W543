// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
#include "Board.h"

namespace {
constexpr int Inset = 30;
constexpr int Samples = 20;
int stage = 0;
int samples = 0;
int32_t sumX = 0, sumY = 0;
bool waitingForRelease = false;
TouchMath::Point firstRaw;
TouchMath::Calibration result{200, 3800, 3800, 200, BoardConfig::SwapTouchAxes};
uint32_t lastPaint = 0;

void target() {
    auto& gfx = Board::screen();
    gfx.fillScreen(Board::Background);
    Board::label(96, 105, stage == 0 ? "1. Hold top-left +" : "2. Hold bottom-right +");
    Board::label(96, 135, "Use a blunt stylus; keep still.", Board::Muted, 1);
    const int x = stage == 0 ? Inset : BoardConfig::Width - 1 - Inset;
    const int y = stage == 0 ? Inset : BoardConfig::Height - 1 - Inset;
    gfx.drawCircle(x, y, 12, Board::Accent);
    gfx.drawFastHLine(x - 18, y, 37, Board::White);
    gfx.drawFastVLine(x, y - 18, 37, Board::White);
    gfx.flush();
}

void finish(TouchMath::Point second) {
    if (!TouchMath::axisFromTargets(firstRaw.x, second.x, Inset, BoardConfig::Width - 1 - Inset,
                                   BoardConfig::Width, result.left, result.right) ||
        !TouchMath::axisFromTargets(firstRaw.y, second.y, Inset, BoardConfig::Height - 1 - Inset,
                                   BoardConfig::Height, result.top, result.bottom)) {
        Serial.println("Calibration rejected: insufficient movement. Start again.");
        stage = 0;
        target();
        return;
    }
    stage = 2;
    Serial.println("Copy these values to include/BoardConfig.h, then rebuild touch_demo:");
    Serial.printf("constexpr int RawLeft = %d;\nconstexpr int RawRight = %d;\n"
                  "constexpr int RawTop = %d;\nconstexpr int RawBottom = %d;\n",
                  result.left, result.right, result.top, result.bottom);
    Serial.printf("constexpr bool SwapTouchAxes = %s;\n", result.swapAxes ? "true" : "false");
    auto& gfx = Board::screen();
    gfx.fillScreen(Board::Background);
    Board::label(16, 16, "Calibration ready");
    Board::label(16, 48, "Values printed to Serial Monitor.", Board::Muted, 1);
    Board::label(16, 66, "Drag to check alignment. Send r to restart.", Board::Muted, 1);
    gfx.flush();
}
}

void setup() {
    if (!Board::begin()) Board::fatal("Initialization failed. See docs/troubleshooting.md.");
    Serial.println("Touch calibration: hold each cross until asked to release.");
    target();
}

void loop() {
    while (Serial.available()) {
        if (Serial.read() == 'r') {
            stage = 0; samples = 0; sumX = 0; sumY = 0;
            waitingForRelease = true;
            target();
        }
    }
    const auto touch = Board::readTouch();
    if (waitingForRelease) {
        if (!touch.down) { waitingForRelease = false; if (stage < 2) target(); }
    } else if (stage < 2) {
        if (!touch.down) { samples = 0; sumX = 0; sumY = 0; }
        else {
            sumX += result.swapAxes ? touch.raw.y : touch.raw.x;
            sumY += result.swapAxes ? touch.raw.x : touch.raw.y;
            if (++samples >= Samples) {
                const TouchMath::Point average{int(sumX / Samples), int(sumY / Samples)};
                samples = 0; sumX = 0; sumY = 0;
                if (stage == 0) { firstRaw = average; stage = 1; }
                else finish(average);
                waitingForRelease = true;
                Serial.println("Release the screen.");
                if (stage < 2) {
                    Board::screen().fillScreen(Board::Background);
                    Board::label(100, 120, "Release the screen");
                    Board::screen().flush();
                }
            }
        }
    } else if (touch.down && millis() - lastPaint >= 50) {
        lastPaint = millis();
        const auto p = TouchMath::map(touch.raw, result, BoardConfig::Width, BoardConfig::Height);
        auto& gfx = Board::screen();
        gfx.fillScreen(Board::Background);
        Board::label(16, 16, "Drag to check. Serial: r = restart.", Board::Muted, 1);
        gfx.drawFastHLine(p.x - 8, p.y, 17, Board::Accent);
        gfx.drawFastVLine(p.x, p.y - 8, 17, Board::Accent);
        gfx.flush();
        Serial.printf("raw: %d,%d | pixel: %d,%d | z: %d\n",
                      touch.raw.x, touch.raw.y, p.x, p.y, touch.pressure);
    }
    delay(10);
}
