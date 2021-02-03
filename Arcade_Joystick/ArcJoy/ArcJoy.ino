/*
    Hardware debounced arcade joystick + 3 buttons + 4 switch positions

    Based on project by Alexey Melnikov
    based on project by Mikael Norrgård <mick@daemonbite.com>

    GNU GENERAL PUBLIC LICENSE
    Version 3, 29 June 2007

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

// ****** a shortcut ******
#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))

// ****** customize ******
// --- use shift register for retro output
#define USE_RETRO

const char *gp_serial = "ArcJoy";

#include <EEPROM.h>
#include "Joystick.h"

Joystick_ Joystick;

// ****** setup µc ******
void setup() {
  // --- set all usable pins to input without pullups, enable pullup on all others
  DDRB  = 0x00;
  PORTB = B10000011;
  DDRC  = 0x00;
  PORTC = B10111111;
  DDRD  = 0x00;
  PORTD = B01101100;
  DDRE  = 0x00;
  PORTE = B10111111;
  DDRF  = B01110000;    // 74hc959 output @ PF4-6
  PORTF = B10001111;
  // --- reset hid report
  Joystick.reset();
}

// ****** handle hid report ******
JoystickReport joy_rep;
void loop() {
  // --- LEDs off
  TXLED1;
  RXLED1;

  // --- buttons to deafult
  joy_rep.buttons = 0x00;

  // --- joystick big buttons
  if (PIND & (1 << PD4)) joy_rep.b0 = 1;
  if (PINC & (1 << PC6)) joy_rep.b1 = 1;
  if (PIND & (1 << PD7)) joy_rep.b2 = 1;
  // --- joystick switches
  if (PIND & (1 << PD0)) joy_rep.b3 = 1;
  if (PIND & (1 << PD1)) joy_rep.b4 = 1;
  if (PINB & (1 << PB3)) joy_rep.b5 = 1;
  if (PINB & (1 << PB6)) joy_rep.b6 = 1;

  // joystick directions
  if (PINB & (1 << PB5)) joy_rep.x = -1; else if (PINB & (1 << PB4)) joy_rep.x = 1; else joy_rep.x = 0;   // left/right
  if (PINE & (1 << PE6)) joy_rep.y = -1; else if (PINB & (1 << PB2)) joy_rep.y = 1; else joy_rep.y = 0;   // up/down

  // --- Only report controller state if it has changed
  if (memcmp(&Joystick._JoystickReport, &joy_rep, sizeof(JoystickReport))) {
    // -- report via usb
    Joystick._JoystickReport = joy_rep;
    Joystick.send();
#ifdef USE_RETRO
    // -- shift out retro joystick connection
    // - prepare first 4 button data
    uint8_t jtmp = joy_rep.buttons << 4;
    // - prepare joystick directions
    if (joy_rep.x == -1) jtmp |= B00000001; // left
    if (joy_rep.x ==  1) jtmp |= B00000010; // right
    if (joy_rep.y == -1) jtmp |= B00000100; // up
    if (joy_rep.y ==  1) jtmp |= B00001000; // down
    // - shift out bits
    for (uint8_t i=0;i<8;i++) {
      if (jtmp & 0x80) {
        sbi (PORTF, PF4);
      } else {
        cbi (PORTF, PF4);
      }
      sbi (PORTF, PF6);   // clock out next bit
      cbi (PORTF, PF6);
      jtmp <<= 1;
    }
    // - copy to storage register/outputs
    sbi (PORTF, PF5);
    cbi (PORTF, PF5);
#endif
  }
}
