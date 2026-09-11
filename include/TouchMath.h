// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2026 Høgni Zachariasen
#pragma once
#include <stdint.h>

namespace TouchMath {
struct Point { int x; int y; };
struct Calibration {
    int left;
    int right;
    int top;
    int bottom;
    bool swapAxes;
    bool valid() const { return left != right && top != bottom; }
};

// Reversed endpoints are intentional: they represent an inverted axis.
inline int mapAxis(int raw, int first, int last, int pixels) {
    if (pixels <= 1 || first == last) return 0;
    const int64_t scaled = (int64_t(raw) - first) * (pixels - 1) / (int64_t(last) - first);
    return scaled < 0 ? 0 : scaled >= pixels ? pixels - 1 : int(scaled);
}

inline Point map(Point raw, const Calibration& c, int width, int height) {
    if (c.swapAxes) { const int x = raw.x; raw.x = raw.y; raw.y = x; }
    return {mapAxis(raw.x, c.left, c.right, width), mapAxis(raw.y, c.top, c.bottom, height)};
}

// Infer raw values at the edges from two inset calibration targets.
inline bool axisFromTargets(int rawFirst, int rawLast, int firstPixel,
                            int lastPixel, int pixels, int& first, int& last) {
    if (pixels < 2 || firstPixel < 0 || lastPixel >= pixels || lastPixel <= firstPixel)
        return false;
    const int64_t span = int64_t(rawLast) - rawFirst;
    if (span > -500 && span < 500) return false; // Same spot / implausible movement.
    const int64_t pixelSpan = lastPixel - firstPixel;
    first = int(rawFirst - span * firstPixel / pixelSpan);
    last = int(rawLast + span * (pixels - 1 - lastPixel) / pixelSpan);
    return true;
}

inline bool contains(Point p, int x, int y, int width, int height) {
    return p.x >= x && p.x < x + width && p.y >= y && p.y < y + height;
}
}
