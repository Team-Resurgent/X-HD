# XboxHDMI-TR

Open-source alternate firmware for MakeMHz's HD+, based on the [Original Xbox 100% Digital HDMI (Prototype)](https://github.com/Ryzee119/XboxHDMI-Ryzee119) by Ryzee119.

**Status:** Refactored to build for STM32F0 (HD+), Cerbios V3.1.0 recommended for improved video mode detection.


## Programming the HD+

Connect the following wires to program the HD+ (if already installed, using the power from the Xbox motherboard is recommended; do NOT use the 3V3 pin below)...

| Pin Pair | Signal | Signal |
|----------|--------|--------|
| 2 – 1    | NC     | NRST   |
| 4 – 3    | NC     | NC     |
| 6 – 5    | GND    | NC     |
| 8 – 7    | SWDIO  | SWCLK  |

**Full pinout reference:**

| Pin Pair | Signal   | Signal |
|----------|----------|--------|
| 2 – 1    | 3V3      | NRST   |
| 4 – 3    | OSC OUT  | TX     |
| 6 – 5    | GND      | RX     |
| 8 – 7    | SWDIO    | SWCLK  |


## Updating firmware with XHDUpdater.xbe (from v0.1.0)

1. Upload all files to your Xbox (e.g. via FTP): `firmware_conexant.bin`, `firmware_focus.bin`, `firmware_xcalibur.bin`, and `XHDUpdater.xbe`.
2. Run the XBE on your Xbox and follow the on-screen prompts.

XHDUpdater source code: [https://github.com/Team-Resurgent/XHDUpdater](https://github.com/Team-Resurgent/XHDUpdater)


## Important Notes

- Backup the existing firmware before attempting to use this (use the ST Link V2 for this)
- This firmware is **NOT** supported by the HD+ creator, so don't got bother MakeMHz for support
- Before attempting to use this, make sure your install works properly with the original FW + patched bios provided by MakeMHz
- Not all features from the original FW are present, bios dependent features like 720p scaling will never be implemented (the adv chip itself doesn't scale the picture).


## To do

- Enhance functionality (on-going)
- Check all video modes render correctly (on-going)

— EqUiNoX
