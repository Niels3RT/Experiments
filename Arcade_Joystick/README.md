## Introduction

A compact frame for an arcade joystick, 3x 24mm buttons and 2x smaller buttons or switches. All axes/buttons/switches are debounced in hardware.

The holes for the 24mm buttons might not be perfectly sized, some of my cheap buttons (the cheapest i could find at Aliexpress) fit tightly, some are loose.

A latched shift register is used to provide outputs that should be compatible to old 8bit computers (untested so far).

![top](overview.png?raw=true "overview")
![bottom](pcb_bottom_label.png?raw=true "bottom pcb")

Red: 100nF caps forgotten in picture, PCB is OK.

## Parts

| part | nr |
|------|-------------|
| Arduino Pro Micro | 1 |
| Micro USB cable | 1 |
| 74HC14 | 2 |
| JST XH2.54 5 | 1 |
| JST XH2.54 3 | 2 |
| JST XH2.54 2 | 3 |
| Resistor 10k | 11 |
| Resistor 2k | 11 |
| Capacitor 100nF | 15 |
| Standoff M3 40mm | 6 |
| Screw M3x6 | 6 |
| Screw M5x8 | 4 |
| Nut M5 | 4 |
| Arcade Joystick | 1 |
| Arcade Buttons 24mm | 3 |
| small Buttons/Switches | 2 |
| optional: 74HC595 | 1 |
| optional: pin header 1x10 | 1 |

## Source parameters

Comment/uncomment USE_RETRO to disable/enable data output to the old computer joystick interface.

## License
This project is licensed under the GNU General Public License v3.0.
