# JC4827W543 Arduino starter

A small, standalone starting point for programming the **JC4827W543 ESP32-S3
display board** with VS Code, PlatformIO and Arduino. Draw on the screen, read
touch input, control the backlight, and build your own interface.

The hardware profile comes from the display and touch configuration used in
eXpander. The examples here are standalone applications with no dependency on
the eXpander firmware, pressure sensor, pump, mobile app or network.

**Supported profile:** 480 × 272 NV3041A display over QSPI, XPT2046 resistive
touch, 4 MB flash and 8 MB OPI PSRAM. Check your board/module markings first.
Capacitive-touch variants need a different touch driver; it is **not implemented**
here. Start with `display_only` if the touch controller is unknown.
See [hardware and pinout](docs/hardware.md).

## What is included?

| Example | What it teaches | Source |
| --- | --- | --- |
| `display_only` | Text, RGB color bars, screen border and uptime | [display_only.cpp](src/display_only.cpp) |
| `touch_demo` (default) | Three touch buttons, counter, colors, brightness and coordinates | [main.cpp](src/main.cpp) |
| `touch_calibration` | Two-point calibration, raw readings and an alignment check | [calibration.cpp](src/calibration.cpp) |

Also included: shared display/touch setup, editable pin and calibration settings,
host tests, a GitHub Actions build workflow, and troubleshooting guides.
No LVGL setup, accounts or API keys are required.

## Quick start

1. Install [Visual Studio Code](https://code.visualstudio.com/),
   [Git](https://git-scm.com/downloads), and the **PlatformIO IDE** extension
   (`platformio.platformio-ide`) in VS Code. Git must be available on `PATH`
   because the touch library is fetched at a fixed commit.
2. On this repository's GitHub page, select **Code → Download ZIP**, extract it,
   and open the extracted folder in VS Code. Open the folder that contains
   `platformio.ini`, not its parent. Cloning the repository works too.
3. Let PlatformIO finish installing. It downloads the pinned framework and
   libraries on the first build; this requires internet access and can take
   several minutes.
4. In the PlatformIO sidebar, expand **Project Tasks → display_only → General**
   and select **Build**. No board is needed for building.
5. Disconnect any external controlled equipment, then connect the display board
   with a **USB data cable**. Uploading replaces the firmware currently installed
   on the board; keep its source or a backup if you need to restore it.
6. Select **Upload** under the same `display_only` environment, then **Monitor**.
   Expect a greeting, red/green/blue/white bars and an increasing uptime counter.
7. For an XPT2046 board, build and upload `touch_demo`. Tap **+1**, **COLOR** and
   **LIGHT**. If touch locations are inaccurate, follow
   [touch calibration](docs/touch-calibration.md).

Always select the intended environment. The general toolbar Build/Upload buttons
use the selected environment, which is `touch_demo` by default.

For command-line use, open **PlatformIO: New Terminal** from the VS Code command
palette and run these commands from the project root:

```sh
pio run -e display_only
pio device list
pio run -e display_only -t upload
pio device monitor -b 115200
```

With multiple serial devices, specify the board's current port explicitly:

```sh
pio run -e touch_demo -t upload --upload-port COM7
pio device monitor --port COM7 -b 115200
```

`COM7` is an example. On Linux or macOS use the port shown by `pio device list`.
Close Serial Monitor before uploading; exit the terminal monitor with `Ctrl+C`.
See [installation and first upload](docs/getting-started.md) for USB recovery.

## Make your first change

Open [src/main.cpp](src/main.cpp), change the `"JC4827W543"` heading in `render()`,
then build and upload `touch_demo`. The loop reads touch, changes application
state and redraws the interface. A button acts once when a new press starts.

Pin assignments, resolution and calibration live in
[include/BoardConfig.h](include/BoardConfig.h). Display and touch initialization
live in [src/Board.cpp](src/Board.cpp). Use `Board::screen()` to access the
Arduino_GFX canvas, and call `flush()` after drawing to send it to the display.
See [extending the examples](docs/development.md).

## Documentation

- [Installation, environment selection and first upload](docs/getting-started.md)
- [Board profile, GPIO pinout and memory configuration](docs/hardware.md)
- [Touch calibration and raw diagnostics](docs/touch-calibration.md)
- [Code structure, dependencies and adding features](docs/development.md)
- [Troubleshooting](docs/troubleshooting.md)
- [Validation results and hardware checklist](docs/validation.md)
- [Third-party notices and upstream references](THIRD_PARTY.md)

```text
JC4827W543/
├── platformio.ini             # Pinned toolchain and selectable examples
├── include/                  # Board configuration, helpers, portable touch math
├── src/                      # Shared board setup and three example applications
├── test/test_touch_math/     # Tests that run on a computer, without a board
├── docs/                     # Setup, pinout, calibration and development guides
├── .github/workflows/        # Automated host tests and all three firmware builds
├── .vscode/extensions.json  # Suggested PlatformIO extension
├── LICENSE                   # GNU GPL version 3 license text
└── THIRD_PARTY.md            # Dependency attribution and licensing boundaries
```

## Validation status

See [the validation record](docs/validation.md) for the build/test results and
remaining physical checks. The pin configuration originates in working eXpander
firmware, but these new standalone examples must still be checked on a physical
board. A successful build does not prove touch calibration or display operation.

## License and credits

Copyright (c) 2026 Høgni Zachariasen.

This starter's original code and documentation are available under the
[GNU General Public License, version 3 or any later version](LICENSE)
(`GPL-3.0-or-later`). This program is distributed without any warranty; see the
license for its full terms. Dependencies retain their own licenses; see
[THIRD_PARTY.md](THIRD_PARTY.md).

Project owner and architect: **Høgni Zachariasen**. Starter implementation and
documentation: **ChatGPT Codex**, based on the eXpander board configuration
developed with GitHub Copilot and ChatGPT Codex. This is a community starter,
not an official board-manufacturer SDK.
