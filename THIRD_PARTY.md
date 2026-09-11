# Third-party dependencies and provenance

The [GNU GPL version 3 or any later version](LICENSE) (`GPL-3.0-or-later`) covers
this repository's original starter source and documentation. It does not
relicense the Arduino framework, graphics library,
touch library, test framework or compiler tools.

Dependencies are downloaded by PlatformIO and are not vendored into this
repository. Their license notices remain with their upstream source distributions:

- [pioarduino platform-espressif32](https://github.com/pioarduino/platform-espressif32/tree/51.03.07)
  distributes the selected Arduino/ESP-IDF platform and build tools.
- [Espressif Arduino-ESP32 3.0.7](https://github.com/espressif/arduino-esp32/tree/3.0.7)
  provides the Arduino framework. Its package identifies LGPL-2.1-or-later;
  bundled components also carry their own notices.
- [Arduino_GFX by Moon On Our Nation](https://github.com/moononournation/Arduino_GFX)
  provides NV3041A, QSPI, framebuffer drawing and its built-in font. Its source
  includes contributions and code with individual upstream notices; consult
  the files used when distributing a combined product.
- [XPT2046_Touchscreen by Paul Stoffregen](https://github.com/PaulStoffregen/XPT2046_Touchscreen/tree/d57f64c8b5f2bc5b8d10d121550806eeff7b06d9)
  provides the resistive-touch driver. Its source contains the MIT-style license
  and the copyright/development-funding notice that must be preserved.
- [Unity by ThrowTheSwitch](https://github.com/ThrowTheSwitch/Unity/tree/v2.6.1)
  provides the host test framework under its MIT license.

The display/touch GPIO mapping and initial calibration values originate in the
eXpander project's working board configuration, owned by Høgni Zachariasen and
developed with GitHub Copilot and ChatGPT Codex. The standalone examples and
guides were written for this starter. No eXpander-specific fonts, calibration
measurements, Wi-Fi credentials, mobile app code, schematics or vendor example
archives are included.

Manufacturer schematics and datasheets are not republished here. Confirm the
electrical documentation for your exact board revision with its supplier.
