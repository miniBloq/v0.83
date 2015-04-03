/*
  wiring_private.h - Internal header file.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

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

  $Id: wiring.h 239 2007-01-12 17:58:39Z mellis $
*/

#ifndef WiringPrivate_h
#define WiringPrivate_h

#include <avr/io.h>
#include <avr/interrupt.h>

//##RG.Labs.20110224 begin
//#include <avr/delay.h>
#include <util/delay.h>
//##RG.Labs.20110224 end

#include <stdio.h>
#include <stdarg.h>

#include "wiring.h"

#ifdef __cplusplus
extern "C"{
#endif

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//##RG.Labs.20110305 begin
// Added the definitions for the MEGA32U4
#if defined(__AVR_ATmega32U4__)

#define EXTERNAL_INT_0 0
#define EXTERNAL_INT_1 1
#define EXTERNAL_INT_2 2
#define EXTERNAL_INT_3 3

#define EXTERNAL_PCINT_0 0
#define EXTERNAL_PCINT_1 1
#define EXTERNAL_PCINT_2 2
#define EXTERNAL_PCINT_3 3
#define EXTERNAL_PCINT_4 4
#define EXTERNAL_PCINT_5 5
#define EXTERNAL_PCINT_6 6
#define EXTERNAL_PCINT_7 7

#define EXTERNAL_NUM_INTERRUPTS 4

#else
//##RG.Labs.20110305 end


#define EXTERNAL_INT_0 0
#define EXTERNAL_INT_1 1
#define EXTERNAL_INT_2 2
#define EXTERNAL_INT_3 3
#define EXTERNAL_INT_4 4
#define EXTERNAL_INT_5 5
#define EXTERNAL_INT_6 6
#define EXTERNAL_INT_7 7

//##RG.Labs.20110305 Begin
#endif
//##RG.Labs.20110305 end

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
#define EXTERNAL_NUM_INTERRUPTS 8
#else
#define EXTERNAL_NUM_INTERRUPTS 2
#endif

typedef void (*voidFuncPtr)(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
