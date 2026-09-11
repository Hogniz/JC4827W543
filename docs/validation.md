# Validation record

[Back to README](../README.md)

## Local validation — 2026-09-11

Executed on Windows from the standalone starter folder, including the space in
its path. Dependencies were installed through this project's `platformio.ini`;
the examples do not reference the eXpander checkout.

The final build and host tests used **PlatformIO Core 6.1.18**, matching the
version selected for GitHub Actions. An initial build/test pass also succeeded
with the installed Core 6.2.0. Core 6.1.18 printed an older-Core notice because
6.2.0 had previously been used on the same computer; this was not a build error.

| Check | Result |
| --- | --- |
| `pio test -e native` | **7 of 7 test cases passed** |
| `pio run -e display_only` | **Passed** |
| `pio run -e touch_demo` | **Passed** |
| `pio run -e touch_calibration` | **Passed** |
| Whitespace and local documentation links | Checked across the source/documentation bundle |
| `git diff --check` | Passed; new files also received a separate whitespace check |
| GitHub Actions execution | Not run here; workflow supplied for the repository |
| Upload / new physical-board validation | **Not performed** |

Final compiler-reported sizes:

| Environment | Static RAM | Application flash |
| --- | ---: | ---: |
| `display_only` | 19,248 bytes | 379,257 bytes |
| `touch_demo` | 20,008 bytes | 390,709 bytes |
| `touch_calibration` | 20,016 bytes | 390,569 bytes |

These are compiler/linker figures. Static RAM usage excludes dynamic allocations
such as the framebuffer; it is not a measurement of runtime free memory.

The selected pioarduino release ZIP was downloaded separately and its platform
metadata matched the installed platform used for the builds. Release `51.03.07`
contains metadata version `51.03.06` (displayed by PlatformIO as `51.3.6`). Its
SHA-256 at validation time was:

```text
aa881a2912ccae03596a581bf620b9396ffcf6957620dc2f260a13818cd33e70
```

This is an observed download checksum, not an additional hash-enforcement step
in PlatformIO. Packages/toolchains already present in the computer's cache were
reused; this was not a clean operating-system installation or a Linux CI run.

## Physical checks still to perform

The GPIO setup originates in the working eXpander display implementation. That
experience does not replace validation of these standalone applications.
Record the exact board revision and memory marking when performing these checks:

- [ ] Upload `display_only`; verify text, border, RGB colors and uptime.
- [ ] Check serial diagnostics report the expected flash and PSRAM.
- [ ] Reboot with no computer attached; verify the application starts.
- [ ] Upload `touch_demo`; check each button once per press and on repeated taps.
- [ ] Hold and release a button; verify it does not keep firing or remain stuck.
- [ ] Check all four brightness levels remain usable.
- [ ] Complete calibration and verify the center plus all four screen corners.
- [ ] Copy calibration values to the header, rebuild and recheck the touch demo.
- [ ] Recover from ROM download mode using the appropriate BOOT/RESET procedure.

Use the bare display board for these checks, with external controlled equipment
disconnected. Upload replaces existing firmware. No eXpander pressure tests or
actuator operations are part of this starter's validation.
