# Hardware profile and GPIO pinout

[Back to README](../README.md)

This repository targets the **JC4827W543 configuration used by eXpander**:

| Component | Configuration |
| --- | --- |
| MCU | ESP32-S3 |
| Display | NV3041A, 480 × 272, landscape rotation 0 |
| Display bus | QSPI, not a parallel RGB panel |
| Touch | XPT2046 resistive controller on a separate SPI bus |
| Memory profile | 4 MB QIO flash, 8 MB OPI PSRAM (N4R8 profile) |
| Serial monitor | USB CDC, 115200 baud |

Check the full board name, PCB revision and module memory marking. The base name
alone does not establish the touch controller. A capacitive JC4827W543 variant
is not supported by the XPT2046 code. Try `display_only` first on a variant with
the same display wiring, and implement its actual touch controller separately.
There is no GT911 driver, I2C touch probing or automatic variant detection here.

Do not apply a guide for a similarly named board merely because it also has a
480 × 272 display. The display controller, bus and wiring must match.

## Internal display wiring

All numbers below are **ESP32 GPIO numbers**, not physical module pin numbers or
connector positions. These components are already connected on the display PCB;
no jumper wires are needed for the examples.

| Signal | GPIO | Used by |
| --- | ---: | --- |
| LCD CS | 45 | NV3041A QSPI chip select |
| LCD SCK | 47 | QSPI clock |
| LCD D0 | 21 | QSPI data 0 |
| LCD D1 | 48 | QSPI data 1 |
| LCD D2 | 40 | QSPI data 2 |
| LCD D3 | 39 | QSPI data 3 |
| LCD backlight | 1 | Active-high PWM, 5 kHz, 8-bit, LEDC channel 2 |
| Touch SCK | 12 | XPT2046 SPI clock |
| Touch MISO | 13 | XPT2046 data to MCU |
| Touch MOSI | 11 | MCU data to XPT2046 |
| Touch CS | 38 | XPT2046 chip select |
| Touch IRQ | 3 | Active-low XPT2046 /PENIRQ |

The NV3041A constructor uses `GFX_NOT_DEFINED` for reset; the example does not
assign a separate display reset GPIO. The display bus uses SPI2_HOST. The pinned
Arduino-ESP32 core maps `SPIClass(HSPI)` on ESP32-S3 to SPI3_HOST, keeping the touch
bus separate. Preserve this separation when changing the implementation.

The working pin assignments are centralized in
[BoardConfig.h](../include/BoardConfig.h). Do not copy a physical module pin
number into that file as if it were a GPIO number.

## Memory and USB settings

[platformio.ini](../platformio.ini) uses PlatformIO's `esp32-s3-devkitc-1` build
definition as a generic ESP32-S3 base, with explicit overrides for this board:

```ini
board_build.arduino.memory_type = qio_opi
board_build.flash_mode = qio
board_build.psram_type = opi
board_upload.flash_size = 4MB
board_upload.maximum_size = 4194304
board_build.partitions = default.csv
```

The build flags enable PSRAM and native USB CDC. Arduino-ESP32's `default.csv`
fits within 4 MB; this starter does not implement OTA updates or use its data
partition. The RGB565 framebuffer alone needs 480 × 272 × 2 = **261,120 bytes**.
Check the serial diagnostics for detected flash and PSRAM sizes. A different
memory variant needs matching settings; changing only the printed board name
does not change the hardware.

USB D-/D+ on ESP32-S3 use GPIO19/GPIO20. Keep them available while using native
USB. These pins, the internal display/touch pins and flash/PSRAM connections are
not a list of spare GPIOs. This guide deliberately does not assign undocumented
external connector pins, SD-card pins or capacitive-touch pins.

## Connecting external circuits

Use the board's documented power input and verify its connector polarity for
your revision. GPIO signals are 3.3 V logic. Do not connect motors, valves or
other power loads directly to GPIOs. The starter configures only the display,
touch and backlight; it provides no pressure regulation or actuator safeguards.

## References

- [Arduino_GFX](https://github.com/moononournation/Arduino_GFX): NV3041A, QSPI and canvas implementations.
- [XPT2046_Touchscreen v1.4](https://github.com/PaulStoffregen/XPT2046_Touchscreen/tree/d57f64c8b5f2bc5b8d10d121550806eeff7b06d9): touch driver used here.
- [Arduino-ESP32 USB CDC documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/usb_cdc.html).
- [pioarduino release 51.03.07](https://github.com/pioarduino/platform-espressif32/releases/tag/51.03.07): selected framework distribution.
