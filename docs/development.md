# Extend the examples

[Back to README](../README.md)

## Source organization

| File | Responsibility |
| --- | --- |
| [BoardConfig.h](../include/BoardConfig.h) | Wiring, dimensions and per-panel calibration |
| [Board.h](../include/Board.h) / [Board.cpp](../src/Board.cpp) | Display, backlight, touch polling and small drawing helpers |
| [TouchMath.h](../include/TouchMath.h) | Hardware-independent coordinate mapping, calibration and hit testing |
| [main.cpp](../src/main.cpp) | Interactive touch demo, built by `touch_demo` |
| [display_only.cpp](../src/display_only.cpp) | Minimal display example, built by `display_only` |
| [calibration.cpp](../src/calibration.cpp) | Guided two-point calibration, built by `touch_calibration` |

PlatformIO's `build_src_filter` chooses one application's `setup()` and `loop()`
alongside `Board.cpp`. Only one example runs at a time. The `display_only`
environment also sets `STARTER_TOUCH_ENABLED=0`, excluding touch initialization
and polling; the shared dependency list can still download the touch library.

## Drawing

After `Board::begin()` succeeds:

```cpp
auto& gfx = Board::screen();
gfx.fillScreen(Board::Background);
Board::label(20, 20, "My application");
gfx.fillCircle(240, 136, 30, Board::Accent);
gfx.flush();
```

The canvas uses 16-bit RGB565 colors and the library's built-in font. Drawing
updates the framebuffer; `flush()` sends it to the physical display. Avoid
flushing for every individual shape. The touch demo redraws at most 20 times
per second when input changes and yields with `delay(10)` between iterations.

## Touch and buttons

`Board::readTouch()` returns `down`, `pressed`, raw coordinates, mapped pixel
coordinates and a raw pressure value. `pressed` is true only on the transition
to a new press. Use it for one-shot buttons; use `down` for dragging or drawing.

```cpp
const auto touch = Board::readTouch();
if (touch.pressed && TouchMath::contains(touch.pixel, 20, 100, 120, 50)) {
    // Change your application state, then redraw.
}
```

At idle, the touch IRQ avoids unnecessary SPI reads. After a press, the driver
continues polling until release. Call it regularly and from a single context.
Long blocking work in `loop()` makes touch unresponsive.

## Backlight and diagnostics

`Board::brightness(value)` accepts 0–255. Zero turns off the backlight, so keep a
visible minimum for controls that must remain usable. This project uses the
Arduino-ESP32 **3.x** API: `ledcAttachChannel(pin, frequency, resolution, channel)`
and `ledcWrite(pin, duty)`. Old 2.x examples using `ledcSetup` and
`ledcAttachPin` are not drop-in replacements.

`Board::diagnostics()` prints the chip, ESP-IDF version, detected memory and free
heap. USB startup waits only 300 ms. Open the monitor before reset, or send `i`
in `touch_demo` if the initial messages were missed.

For larger applications, keep all display and touch access in one task or
serialize it deliberately. The starter does not add its own FreeRTOS tasks,
network stack, filesystem, sensor drivers, Bluetooth API or persistent settings.
Add one subsystem at a time after the display/touch baseline works.

## Pinned dependencies

| Dependency | Selected version |
| --- | --- |
| pioarduino platform | Release `51.03.07` ZIP (metadata reports `51.3.6`) |
| Arduino-ESP32 | `3.0.7`, provided by that platform |
| Arduino_GFX / GFX Library for Arduino | `1.4.9` |
| XPT2046_Touchscreen | v1.4 commit `d57f64c8b5f2bc5b8d10d121550806eeff7b06d9` |
| PlatformIO native platform | `1.2.1` |
| Unity host test framework | `2.6.1` |
| PlatformIO Core in CI | `6.1.18` |

This is a fixed compatibility baseline, not a claim that these are the latest
releases. PlatformIO installs dependencies automatically. Keep `.pio/` out of
Git. Updates should be deliberate: build every environment, run the host tests,
and repeat the display/touch checks before changing the pins or toolchain.

## Tests and automation

```sh
pio test -e native
pio run -e display_only -e touch_demo -e touch_calibration
git diff --check
```

Host tests require a native GCC/G++ compiler on PATH. On Windows, use your MinGW
installation; for example in PowerShell:

```powershell
$env:PATH = 'C:\MinGW\bin;' + $env:PATH
pio test -e native
```

That path is an example, not a prerequisite for firmware builds. Linux CI supplies
GCC. The tests cover coordinate edges, inversion, swapping, clipping, invalid
ranges, two-point calibration and button boundaries. They do not test physical
SPI transactions or the panel.

[The GitHub workflow](../.github/workflows/build.yml) runs the tests and all three
builds on pushes and pull requests. It does not upload to hardware, publish
releases or require secrets. See [validation](validation.md) for actual results.

## Publishing your copy

Create the GitHub repository using your preferred Git client, add these files,
commit and push. Include the dotfiles, especially `.github/` and `.vscode/`.
Build outputs and downloaded libraries are ignored. Users can then download a
ZIP or clone the repository and follow the README. The starter's original files
are licensed under GPL-3.0-or-later. Keep the license, copyright and dependency
notices when sharing a modified starter, and follow the GPL's corresponding-source
requirements when distributing firmware. See [LICENSE](../LICENSE) and
[THIRD_PARTY.md](../THIRD_PARTY.md).
