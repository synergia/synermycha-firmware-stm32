# Synermycha Firmware for embedded STM32

_Welcome to the Synermycha Firmware repository!_

## To do list:
- [ ] Timer list (purpose, timing)
- [ ] Create generic menu
- [ ] BLE services setup
- [ ] IMU handling
- [ ] STUSB controll 
- [ ] Encapsulate display into class
- [ ] Motor class
- [ ] Encoder class
- [x] Clang format
- [ ] Oled on DMA
- [ ] PID Motor


To proper working of clang-format formatter, you should have installed clang-format-10
and xaver.clang-format extension for vscode.

To install clang-format-10, type:
**sudo apt install clang-format-10**
then check where it is installed:
**whereis clang-format-10**
It should be installed in /usr/bin/clang-format-10. If not, please fill "clang-format.executable" field in settings.json
with proper path.

Also please install clang-format by xaver extension.

Restart vscode. Now after saving, clang-format will automaticly format code.
