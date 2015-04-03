/*

ToneDelay - Library for blocking Tone
Author: Federico Lanza

Changes:
  V0.1: Nov 2012                First release

*/

#include "toneDelay.h"

void toneWithDelay (uint8_t _pin, unsigned int frequency, unsigned long duration)
{
  unsigned long TimeMs, TimeUs;
  unsigned int Micros;
  int PinValue;
  
  PinValue = 0;
  TimeMs = millis();
  Micros = 500000L / frequency;
  while (!((millis() - TimeMs) >= duration)) {
    TimeUs = micros();
    while (!((micros() - TimeUs) >= Micros));
    PinValue = ~PinValue;
    if (frequency) digitalWrite(_pin, PinValue);
  }
  digitalWrite(_pin, 0);
}
