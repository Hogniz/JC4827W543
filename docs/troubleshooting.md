# Troubleshooting

[Back to README](../README.md)

| Symptom | What to check |
| --- | --- |
| `pio` is not recognized | Open PlatformIO's own terminal in VS Code. The extension includes Core. |
| Git dependency cannot install | Run `git --version`; install Git and restart VS Code if it is missing. Check internet/proxy access to GitHub. |
| First build appears slow | Toolchains and libraries are downloading. Read the terminal output and allow it to complete. |
| `ledcAttachChannel` is undefined | Verify the exact platform URL and Arduino-ESP32 3.0.7. A stock/local 2.x platform is incompatible with this code. |
| No USB port | Use a data cable, another USB port and the correct board connector. Check Device Manager or `pio device list`. |
| Upload remains at `Connecting...` | Close Serial Monitor; enter BOOT/RESET download mode and rediscover the port. See [first upload](getting-started.md). |
| Upload is unreliable | Try a shorter cable and lower `upload_speed` to `115200`. |
| Upload succeeds but there is no output | Press RESET/EN, rediscover the application USB port, open at 115200 baud. Send `i` in `touch_demo` to repeat diagnostics. |
| Black screen | Start with `display_only`. Check the NV3041A QSPI variant, backlight GPIO1, memory settings and serial initialization errors. |
| Backlight works but image is wrong | Verify QSPI pins and display controller. The NV3041A setup is not an RGB-panel configuration. |
| PSRAM is missing / boot loop | Check actual module memory marking against 4 MB flash / 8 MB OPI PSRAM and `qio_opi`. Do not guess memory mode. |
| `Initialization failed` repeats | Inspect display/memory setup first. For touch builds also check the calibration endpoints are unequal. A successful driver initialization does not prove the panel is connected. |
| Touch never responds | Confirm XPT2046 resistive hardware, GPIO38 CS, GPIO3 IRQ, SPI pins, and touch rotation 0. Capacitive touch is unsupported. |
| Touch is offset, mirrored or axes are swapped | Run [calibration](touch-calibration.md), including its drag check; rebuild after editing the header. |
| Touch buttons appear stuck | Keep polling during a held press. Avoid long blocking work and concurrent calls to the touch driver. |
| A different demo runs after upload | Check the selected environment; specify `-e display_only`, `-e touch_demo` or `-e touch_calibration` explicitly. |
| Host tests cannot find `g++` | Install a host compiler or add its directory to PATH. Firmware builds use a separate downloaded cross-compiler. |

After a dependency/configuration change, clean the selected build and rebuild:

```sh
pio run -e touch_demo -t clean
pio run -e touch_demo
```

If reporting an issue, include the full board/module marking, touch type,
environment name, operating system, `pio --version`, complete relevant error
message and the diagnostics output. A photo of the display and board markings
helps distinguish variants. Do not include account credentials or personal data.
