// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
#include "Board.h"
#if STARTER_TOUCH_ENABLED
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#endif

namespace {
using namespace BoardConfig;
Arduino_ESP32QSPI bus(DisplayCs, DisplayClock, DisplayD0, DisplayD1, DisplayD2, DisplayD3);
Arduino_NV3041A display(&bus, GFX_NOT_DEFINED, 0, true);
Arduino_Canvas canvas(Width, Height, &display);
#if STARTER_TOUCH_ENABLED
// QSPI uses SPI2_HOST. In the pinned ESP32-S3 Arduino core, HSPI uses SPI3_HOST.
SPIClass touchSpi(HSPI);
XPT2046_Touchscreen touch(TouchCs, TouchIrq);
bool wasDown = false;
#endif
const TouchMath::Calibration calibration{RawLeft, RawRight, RawTop, RawBottom, SwapTouchAxes};
}

namespace Board {
bool begin() {
    Serial.begin(115200);
    delay(300); // Bounded wait: the demo also boots without a USB host.
    if (!ledcAttachChannel(Backlight, 5000, 8, BacklightChannel)) return false;
    brightness(0);
    if (!canvas.begin()) return false;
    canvas.setTextWrap(false);
    canvas.fillScreen(Background);
    canvas.flush();
#if STARTER_TOUCH_ENABLED
    touchSpi.begin(TouchClock, TouchMiso, TouchMosi, TouchCs);
    if (!touch.begin(touchSpi) || !calibration.valid()) return false;
    touch.setRotation(0);
#endif
    brightness(200);
    diagnostics();
    return true;
}

Arduino_Canvas& screen() { return canvas; }
void brightness(uint8_t value) { ledcWrite(Backlight, value); }

Touch readTouch() {
    Touch result;
#if STARTER_TOUCH_ENABLED
    // Once pressed, continue polling until release even if /PENIRQ is high.
    if (!wasDown && !touch.tirqTouched()) return result;
    result.down = touch.touched();
    result.pressed = result.down && !wasDown;
    if (result.down) {
        const TS_Point p = touch.getPoint();
        result.raw = {p.x, p.y};
        result.pressure = p.z; // Relative raw reading, not a physical unit.
        result.pixel = TouchMath::map(result.raw, calibration, Width, Height);
    }
    wasDown = result.down;
#endif
    return result;
}

void diagnostics() {
    Serial.printf("\nJC4827W543 starter | ESP-IDF %s\n", ESP.getSdkVersion());
    Serial.printf("Chip: %s rev %u | CPU: %u MHz\n", ESP.getChipModel(),
                  ESP.getChipRevision(), ESP.getCpuFreqMHz());
    Serial.printf("Flash: %u bytes | PSRAM: %u bytes | free heap: %u bytes\n",
                  ESP.getFlashChipSize(), ESP.getPsramSize(), ESP.getFreeHeap());
    Serial.printf("Display: %dx%d NV3041A | XPT2046: %s\n", Width, Height,
                  STARTER_TOUCH_ENABLED ? "enabled" : "disabled");
    if (ESP.getFlashChipSize() != 4 * 1024 * 1024 || ESP.getPsramSize() != 8 * 1024 * 1024)
        Serial.println("Memory differs from the 4 MB flash / 8 MB PSRAM profile; check module marking.");
}

void label(int x, int y, const char* text, uint16_t color, uint8_t size) {
    canvas.setTextColor(color);
    canvas.setTextSize(size);
    canvas.setCursor(x, y);
    canvas.print(text);
}

void button(int x, int y, int width, int height, const char* text) {
    canvas.fillRoundRect(x, y, width, height, 8, Panel);
    canvas.drawRoundRect(x, y, width, height, 8, Accent);
    label(x + 12, y + (height - 16) / 2, text);
}

void fatal(const char* message) {
    // Repeat so errors remain visible if USB enumeration finishes after setup().
    while (true) { Serial.println(message); delay(1000); }
}
}
