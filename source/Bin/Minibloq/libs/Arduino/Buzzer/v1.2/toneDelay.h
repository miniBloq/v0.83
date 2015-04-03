#ifndef toneDelay_h
#define toneDelay_h

/*

ToneDelay - Library for blocking Tone
Author: Federico Lanza

Changes:
  V0.1: Nov 2012                First release

*/

#include "Arduino.h"

void toneWithDelay(uint8_t _pin, unsigned int frequency, unsigned long duration);

#endif
