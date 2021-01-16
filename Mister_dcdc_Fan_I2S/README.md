## Introduction
A pcb for Mister featuring a dcdc converter, fan, reset button, power/disk/user leds and an i2s module for line out.

![top](top.jpg?raw=true "top")

5V power supply to the de10-nano is soldered to the back of the connector.
A L78L33ACU linear regulator supplies power to the PCM5102 module, filtering some noise.
And a schottky diode from the parts bin to drop the voltage for the 5V fan a little.

![dcdc](dcdc.jpg?raw=true "dcdc")

The DCDC converter used.

## License
This project is licensed under the GNU General Public License v3.0.
