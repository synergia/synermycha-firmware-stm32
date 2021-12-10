![Build](https://github.com/synergia/synermycha-firmware-stm32/actions/workflows/main.yml/badge.svg)
[![GitHub tag](https://img.shields.io/github/tag/synergia/synermycha-firmware-stm32.svg)](https://github.com/synergia/synermycha-firmware-stm32/tags/)
[![MIT license](https://img.shields.io/badge/License-MIT-blue.svg)](https://lbesson.mit-license.org/)
# Synermycha Firmware for embedded STM32

_Welcome to the Synermycha Firmware repository!_

## Timer configuration:
| Timer         | Bus       | Freq      | Used by               |
| ------------- | --------- | --------- | --------------------- | 
| TIM3          | APB1      | X         | Encoders              | 
| TIM4          | APB1      | X         | Encoders              | 
| TIM9          | APB2      | vary      | Eye led, buzzer       | 
| TIM10         | APB2      | 100Hz     | PID                   | 
| TIM11         | APB2      | 100Hz     | Trajectory            | 
| TIM12         | APB1      | 180kHz    | Motors                | 
| TIM13         | APB1      | 60Hz      | BLE, display, debug   | 
| TIM14         | APB1      | 25Hz      | Distances ToF         | 

APB1 and APB2 for timers have 180MHz.

## RN4871
We've found that handling RN4871 communication don't have very good documentation, 
especially in cases when module sends some messages about characteristic update. Below we present information about module behaviour that is not clearly stated in datasheet.

### RN4871 command return format
 AOK return message: `AOK␍␊`

 ERR return message: `Err␍␊`

 AOK and ERR return messages are followed by: `CMD> `<- with space

### RN4871 message format
 RN4871 message format: `%COMMAND,HANDLE,VALUE%` or `%MESSAGE%`
 no `\r` or `\n` at the end of message

 Connected message: `%CONNECT,1,MAC_ADD%`
 
 Connection parameters info: `%CONN_PARAM,????,????,????%` followed by `%CONN_PARAM,????,????,????%`

 Disonnected message: `%DISCONNECT%`

 Characteristic update `%WV,HANDLE,VALUE%`

 Characteristic notify enable/disable `%WC,HANDLE,0100%` / `%WC,HANDLE,0000%`


## Clang format 
To proper working of clang-format formatter, you should have installed clang-format-10
and xaver.clang-format extension for vscode.

To install clang-format-10, type:
`sudo apt install clang-format-10`

You can check where it is installed:
`whereis clang-format-10`
By default it will be installed in `/usr/bin/clang-format-10`. If not, please set "clang-format.executable" field in settings.json
path obtained from `whereis` command.

Install clang-format by xaver extension.

Restart vscode. 

Now after saving, clang-format will automaticly format code.
