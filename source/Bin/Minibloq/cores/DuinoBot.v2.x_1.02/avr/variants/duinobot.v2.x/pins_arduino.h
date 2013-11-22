/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h

#include <avr/pgmspace.h>

#define BILED_INIT		DDRA |= (1<<7)
#define BILED_RED		PORTA &= ~(1<<7)
#define BILED_GREEN		PORTA |= (1<<7)

#define NUM_DIGITAL_PINS            30
#define NUM_ANALOG_INPUTS           7
#define analogInputToDigitalPin(p)  ((p < 7) ? (p) + 14 : -1)

#define digitalPinHasPWM(p)         ((p) == 9 || (p) == 10 || (p) == 12 || (p) == 13 || (p) == 27 || (p) == 28)

static const uint8_t SS   = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 13;

static const uint8_t SDA = 3;
static const uint8_t SCL = 2;
static const uint8_t LED_BUILTIN = 13;

static const uint8_t A0 = 14;
static const uint8_t A1 = 15;
static const uint8_t A2 = 16;
static const uint8_t A3 = 17;
static const uint8_t A4 = 18;
static const uint8_t A5 = 19;
static const uint8_t A6 = 20; // Battery Status

static const uint8_t BATTERY = 20;
static const uint8_t BILED = 21;
static const uint8_t SPEAKER = 22;

// Motors
static const uint8_t M0_EN = 27;
static const uint8_t M0_D0 = 23;
static const uint8_t M0_D1 = 24;

static const uint8_t M1_EN = 28;
static const uint8_t M1_D0 = 25;
static const uint8_t M1_D1 = 26;

// Run Switch
static const uint8_t RUN_SW = 29;


//## Modificar
#define digitalPinToPCICR(p)    (((p) >= 0 && (p) <= 21) ? (&PCICR) : ((uint8_t *)0))
#define digitalPinToPCICRbit(p) (((p) <= 7) ? 2 : (((p) <= 13) ? 0 : 1))
#define digitalPinToPCMSK(p)    (((p) <= 7) ? (&PCMSK2) : (((p) <= 13) ? (&PCMSK0) : (((p) <= 21) ? (&PCMSK1) : ((uint8_t *)0))))
#define digitalPinToPCMSKbit(p) (((p) <= 7) ? (p) : (((p) <= 13) ? ((p) - 8) : ((p) - 14)))
//##

#ifdef ARDUINO_MAIN

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint16_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &DDRA,
	(uint16_t) &DDRB,
	(uint16_t) &DDRC,
	(uint16_t) &DDRD,
};

const uint16_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PORTA,
	(uint16_t) &PORTB,
	(uint16_t) &PORTC,
	(uint16_t) &PORTD,
};

const uint16_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PORT,
	(uint16_t) &PINA,
	(uint16_t) &PINB,
	(uint16_t) &PINC,
	(uint16_t) &PIND,
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {
	PD, // D0 - PD0
	PD, // D1 - PD1
	PC, // D2 - PC0
	PC, // D3 - PC1
	PC, // D4 - PC2
	PC, // D5 - PC3
	PC, // D6 - PC4
	PC, // D7 - PC5
	PC, // D8 - PC6
	PD, // D9 - PD6
	PB, // D10 - SS - PB4
	PB, // D11 - MOSI - PB5
	PB, // D12 - MISO - PB6
	PB, // D13 - SCK - PB7
	PA, // D14 - A0 - PA0
	PA, // D15 - A1 - PA1
	PA, // D16 - A2 - PA2
	PA, // D17 - A3 - PA3
	PA, // D18 - A4 - PA4
	PA, // D19 - A5 - PA5
	PA, // D20 - A6 - PA6 (Battery Status)
	PA, // D21 - BILED - PA7
	PD, // D22 - SPEAKER - PD7
	PB, // D23 - PB0 - M0_D0
	PB, // D24 - PB1 - M0_D1
	PB, // D25 - PB3 - M1_D0
	PB, // D26 - PB2 - M1_D1
	PD, // D27 - PD5 - M0_EN
	PD, // D28 - PD4 - M1_EN
	PC, // D29 - PC7 - RUN_SW
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
	_BV(0), // PD0
	_BV(1), // PD1
	_BV(0), // PC0..PC6
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(6), // PD6
	_BV(4), // PB4..PB7
	_BV(5),
	_BV(6),
	_BV(7),
	_BV(0), // PA0..PA6
	_BV(1),
	_BV(2),
	_BV(3),
	_BV(4),
	_BV(5),
	_BV(6),
	_BV(7), // PA7
	_BV(7), // PD7
	_BV(0), // PB0
	_BV(1), // PB1
	_BV(3), // PB3
	_BV(2), // PB2
	_BV(5), // PD5
	_BV(4), // PD4
	_BV(7), // PC7
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
// ## Modificar
	NOT_ON_TIMER, // PD0
	NOT_ON_TIMER, // PD1
	NOT_ON_TIMER, // PC0..PC6
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	
	NOT_ON_TIMER, // PD6
	NOT_ON_TIMER, // PB4..PB7
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	
	NOT_ON_TIMER, // PA7

	NOT_ON_TIMER, // PD7

	NOT_ON_TIMER, // PA0..PA6
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,
	NOT_ON_TIMER,

	NOT_ON_TIMER, // PB0
	NOT_ON_TIMER, // PB1
	NOT_ON_TIMER, // PB3
	NOT_ON_TIMER, // PB2

	TIMER1A, // PD5
	TIMER1B, // PD4
	NOT_ON_TIMER,  // PC7
// ##
};

#endif

#endif
