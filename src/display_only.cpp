// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
// No touch initialization. Start here when identifying an unknown touch variant.
#include "Board.h"

void setup() {
    if (!Board::begin()) Board::fatal("Display initialization failed. See docs/troubleshooting.md.");
    auto& gfx = Board::screen();
    Board::label(20, 20, "Hello, JC4827W543!", Board::White, 3);
    Board::label(20, 60, "480 x 272 / NV3041A QSPI", Board::Muted, 2);
    const uint16_t colors[] = {0xF800, 0x07E0, 0x001F, 0xFFFF};
    for (int i = 0; i < 4; ++i) gfx.fillRect(20 + i * 110, 100, 100, 72, colors[i]);
    gfx.drawRect(0, 0, BoardConfig::Width, BoardConfig::Height, Board::Accent);
    gfx.flush();
}

void loop() {
    static uint32_t last = 0;
    const uint32_t now = millis();
    if (now - last >= 1000) {
        last = now;
        auto& gfx = Board::screen();
        gfx.fillRect(20, 204, 440, 32, Board::Background);
        char line[48];
        snprintf(line, sizeof(line), "Uptime: %lu seconds", (unsigned long)(now / 1000));
        Board::label(20, 210, line);
        gfx.flush();
        Serial.println(line);
    }
    delay(10);
}
