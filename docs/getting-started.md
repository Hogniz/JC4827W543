# Installation and first upload

[Back to README](../README.md)

## Required hardware and software

- JC4827W543 with the [documented hardware profile](hardware.md).
- A USB cable that carries data, and a computer running Windows, Linux or macOS.
- VS Code, PlatformIO IDE and Git. The extension includes PlatformIO Core; a
  separate Arduino IDE installation is not needed.
- Internet access for the first toolchain/library download. Subsequent builds
  can reuse the local packages.

The examples run independently on the board after upload. They do not need a
permanent USB connection to a computer, apart from USB power and diagnostics.

## Open and build

Open this repository folder using **File → Open Folder** in VS Code. Wait for
PlatformIO's initialization to finish. Use the PlatformIO sidebar's **Project
Tasks** to choose an environment, then **General → Build**.

Start with `display_only`, progress to `touch_demo`, and use `touch_calibration`
when needed. Do not use PlatformIO's New Project wizard for this checkout: the
board settings and dependencies are already in [platformio.ini](../platformio.ini).

For CLI commands, select **PlatformIO: New Terminal** from the command palette.
If that command is not available, use PlatformIO's sidebar **Quick Access →
Miscellaneous → PlatformIO Core CLI**. Run `pio --version` to check the terminal.
The `pio` command need not be on the regular Windows terminal's PATH when using
the extension's terminal.

```sh
pio run -e display_only
pio run -e touch_demo
pio run -e touch_calibration
```

The pinned pioarduino release provides Arduino-ESP32 3.0.7. Its platform metadata
may print `51.3.6` even though the download release is `51.03.07`. Keep the exact
release URL rather than substituting the stock or locally installed platform.

## Connect and upload

For these starter examples, disconnect pumps, valves and other equipment from
the display board. The demo does not implement their control or startup behavior.
Connect USB, then identify the new serial port with `pio device list` or Windows
Device Manager. Use the USB connector intended for programming/data on your
particular board revision.

```sh
pio run -e display_only -t upload --upload-port COM7
pio device monitor --port COM7 -b 115200
```

Replace `COM7` with the actual port. Port names may change when the device moves
between the bootloader and the application. `upload_port` is deliberately not
hard-coded in the repository.

If upload stays at `Connecting...`, use the board's BOOT and RESET/EN buttons,
if fitted:

1. Hold **BOOT** down.
2. Briefly press and release **RESET/EN**, then release **BOOT**.
3. Find the bootloader port and retry upload using that port.
4. Press RESET/EN after upload if the application does not start automatically.

An unsuccessful sketch normally does not prevent ROM bootloader recovery. Do
not short unidentified pins if your board revision has different buttons.
See Espressif's [boot mode selection guide](https://docs.espressif.com/projects/esptool/en/latest/esp32s3/advanced-topics/boot-mode-selection.html).

## Check the result

`display_only` draws four color bars, a border and an uptime counter.
`touch_demo` displays a counter and three controls:

| Control | Expected behavior |
| --- | --- |
| +1 | Adds one per new press; wraps after 9999 |
| COLOR | Cycles through three accent colors |
| LIGHT | Cycles through four backlight levels, never completely dark |

In the touch demo's Serial Monitor, send `i` to repeat chip/memory information or
`r` to reset the counter. Settings and the counter reset on reboot. Serial startup
is bounded, so the application does not wait forever for a computer.

The calibrated origin is the display's top-left corner: x increases rightward,
y downward. Touch values may initially be offset; calibration is panel-specific.

PlatformIO's [official VS Code guide](https://docs.platformio.org/en/latest/integration/ide/vscode.html)
describes the extension and its built-in terminal.
