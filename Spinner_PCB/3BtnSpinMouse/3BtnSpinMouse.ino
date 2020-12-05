/*
    A2600 Paddles/Spinners USB Adapter
    (C) Alexey Melnikov

    Based on project by Mikael Norrgård <mick@daemonbite.com>

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

///////////////// Customizable settings /////////////////////////

// Spinner pulses per revolution
// For arduino shield spinner: 20
//#define SPINNER_PPR 20
#define SPINNER_PPR 600

// upper limit for debounce counter
// set to 1 to disable debouncing
// 16 seems ok, but sometimes still bounces on my setup
#define DEBOUNCE_TOP 32

///////////////// a shortcut ////////////////////////////////////

#define cbi(sfr, bit)     (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit)     (_SFR_BYTE(sfr) |= _BV(bit))

/////////////////////////////////////////////////////////////////

// pins map
const int8_t encpin[2] = {1, 0};        // rotary encoder

////////////////////////////////////////////////////////

#ifndef SPINNER_SENSITIVITY
#if SPINNER_PPR < 50
#define SPINNER_SENSITIVITY 1
#else
#define SPINNER_SENSITIVITY 2
#endif
#endif

// ID for special support in MiSTer
// ATT: 20 chars max (including NULL at the end) according to Arduino source code.
// Additionally serial number is used to differentiate arduino projects to have different button maps!
//const char *gp_serial_spin = "MiSTer-S1 Spinner";
const char *gp_serial = "3btnSpMouse V0.1";

#include <EEPROM.h>
#include "Spinner.h"
#include "Mouse.h"

Spinner_ Spinner;
SMouse_ SMouse;

uint8_t cnt_btn[3]  = { DEBOUNCE_TOP, DEBOUNCE_TOP, DEBOUNCE_TOP };

uint16_t drvpos;

#define SP_MAX ((SPINNER_PPR*4*270UL)/360)
const uint16_t sp_max = SP_MAX;
int32_t sp_clamp = SP_MAX / 2;

void drv_proc(void)
{
  static int8_t prev;
  int8_t a = digitalRead(encpin[0]);
  int8_t b = digitalRead(encpin[1]);

  int8_t spval = (b << 1) | (b ^ a);
  int8_t diff = (prev - spval) & 3;

  if (diff == 1)
  {
    drvpos += 10;
    if (sp_clamp < sp_max) sp_clamp++;
  }
  if (diff == 3)
  {
    drvpos -= 10;
    if (sp_clamp > 0) sp_clamp--;
  }

  prev = spval;
}

void drv0_isr()
{
  drv_proc();
}

void drv1_isr()
{
  drv_proc();
}

void setup()
{
  float snap = .01;
  float thresh = 8.0;

  // set all usable pins to input with pullups
  DDRB  &= ~B01111110;
  PORTB |=  B01111110;
  DDRC  &= ~B01000000;
  PORTC |=  B01000000;
  DDRD  &= ~B10011111;
  PORTD |=  B10011111;
  DDRE  &= ~B01000000;
  PORTE |=  B01000000;
  DDRF  &= ~B11110000;
  PORTF |=  B11110000;

  Spinner.reset();
  SMouse.reset();

  drv_proc();
  drvpos = 0;
  attachInterrupt(digitalPinToInterrupt(encpin[0]), drv0_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encpin[1]), drv0_isr, CHANGE);
}

SpinnerReport spin_rep;
SMouseReport mouse_rep;
const int16_t sp_step = (SPINNER_PPR * 10) / (20 * SPINNER_SENSITIVITY);
void loop()
{
  // LEDs off
  TXLED1;
  RXLED1;

  // paddle emulation
  spin_rep.paddle = ((sp_clamp * 255) / sp_max);

  // spinner/wheel rotation
  static uint16_t prev = 0;
  int16_t val = ((int16_t)(drvpos - prev)) / sp_step;
  if (val > 127) val = 127; else if (val < -127) val = -127;
  spin_rep.spinner = val;
  prev += val * sp_step;

  ////// spinner //////
  spin_rep.buttons = 0x000;

  // Spinner buttons
  if (!(PIND & (1 << PD7))) cnt_btn[0]  = 0; else if (cnt_btn[0]  < DEBOUNCE_TOP) cnt_btn[0]++;   // btn0
  if (!(PIND & (1 << PD0))) cnt_btn[1]  = 0; else if (cnt_btn[1]  < DEBOUNCE_TOP) cnt_btn[1]++;   // btn1
  if (!(PIND & (1 << PD4))) cnt_btn[2]  = 0; else if (cnt_btn[2]  < DEBOUNCE_TOP) cnt_btn[2]++;   // btn2
  if (cnt_btn[0] < DEBOUNCE_TOP) spin_rep.buttons |= 0x001;
  if (cnt_btn[1] < DEBOUNCE_TOP) spin_rep.buttons |= 0x002;
  if (cnt_btn[2] < DEBOUNCE_TOP) spin_rep.buttons |= 0x004;

  // Only report controller state if it has changed
  if (memcmp(&Spinner._SpinnerReport, &spin_rep, sizeof(SpinnerReport)))
  {
    Spinner._SpinnerReport = spin_rep;
    Spinner.send();
  }

  ////// mouse //////
  mouse_rep.x = val;
  mouse_rep.y = val;

  // Only report controller state if it has changed
  if (memcmp(&SMouse._SMouseReport, &mouse_rep, sizeof(SMouseReport)))
  {
    SMouse._SMouseReport = mouse_rep;
    SMouse.send();
  }
}
