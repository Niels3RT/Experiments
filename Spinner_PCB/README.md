## Introduction
A pcb for [Sorgeligs PaddleTwoControllersUSB project](https://github.com/MiSTer-devel/Retro-Controllers-USB-MiSTer/tree/master/PaddleTwoControllersUSB).

The mounting screws for the encoder should be around M3x6mm, 10mm are just a little bit too long.

Nice big knobs can be borrowed from HiFi equipment or kitchen tools (stoves etc.). Put weight into the knob (nuts, bolts, steel balls, lead) to get the precision tool feel.

![top](top.jpg?raw=true "top")
![bottom](bottom.jpg?raw=true "bottom")

Use a piece of perfboard to prevent pins from sticking out from the top surface.
![arduino_soldering](arduino_soldering.jpg?raw=true "arduino_soldering")

40mm knob, steel balls, hot glue, 60g.
![knob](knob.jpg?raw=true "knob")

The resistors for the leds should be somewhere around 470Ohm to 2kOhm, depending on the led and brightness.
![connections](connections.jpg?raw=true "connections")

## Alternative Firmware (experimental)
The pcb was designed for Sorgeligs firmware, but i included a firmware that does not identify as Mr.Spinner and reports the spinner as a mouse too. This allows for using/mapping all three buttons and might help on other systems, but might cause trouble too...

To configure the spinner/paddle with this firmware in Mister go to "define joystick buttons", first press a button to select it as joystick to configure, then use A->right microswitch, B->left microswitch and X->big button, skip all other button/dpad by pressing space on keyboard.

C64, "swap joysticks: no", "pot 1&2: Paddles 1&2" to use Paddle in Arkanoid.

Atari2600, "define atari2600 buttons", press a microswitch to identify the paddle, skip all other buttons but "Paddle Btn". Use keyboard for start/reset/select etc. In menu use "swap Joystick" to control player 1/2.

Arcade Arkanoid, in "define a.arkanoid buttons" rotate spinner until joystick is selected and right is defined, skip all till "fire"->big button, Start P1->left microswitch, "coin"->right microswitch.

## License
This project is licensed under the GNU General Public License v3.0.
