/*************************************************** 
  This is an example for the Adafruit NEC Remote Control

  Designed specifically to work with the Adafruit NEC Remote Control
    ----> http://www.adafruit.com/products/389
  and IR Receiver Sensor
    ----> http://www.adafruit.com/products/157

  These devices use IR to communicate, 1 pin is required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "IRRemote.h"
#include "pins_arduino.h"
#include "wiring_private.h"


IRRemote::IRRemote(uint8_t pin) {
  _irpin = pin;
  _irpinport = digitalPinToPort(_irpin);
  _irpinmask = digitalPinToBitMask(_irpin);
  

  
  timedout = false;
  timing = false;
  maxwaiting = 0;
}

int16_t IRRemote::listen(int16_t maxwaitseconds) {
  uint16_t p1, p2, timer;
  
  p1 = p2 = 0;
  timer = 0; 
  maxwaiting = maxwaitseconds;
  maxwaiting *= 1000;  // to milliseconds
  maxwaiting *= 1000;  // to microseconds
  
  if (maxwaitseconds) timing = true;
  timedout = false;
  
  // wait for starting pulses
  while ( ((p1 < 8000/RESOLUTION) || (p1 > 10000/RESOLUTION)) && 
          ((p2 < 4000/RESOLUTION) || (p2 > 5000/RESOLUTION)))  {
    p1 = measurePulse(0);
    if (timing) {
        maxwaiting -= p1;
        if (maxwaiting < 0) {
          timedout = true;     // timed out waiting
        }
    }
    if (timedout)           return -1;

    p2 = measurePulse(1);     
    if (timing) {
        maxwaiting -= p2;
        if (maxwaiting < 0) {
          timedout = true;     // timed out waiting
        }
    }
    if (timedout)           return -1;
  }

  //Serial.println("NEC");
  // get 'data'
  uint8_t data[4] = {0, 0, 0, 0};
  int8_t b;
  for (uint8_t i=0; i<32; i++) {
    b = readNECbit();
    if (timedout) {
      return -1;
    }
    if  (b < 0) {
      if (i == 0) 
    return -3; // repeat!
      else
    return -1;
    }
    data[i/8] |= b << (i%8);
  }
  /*
  Serial.print(data[0], HEX); Serial.print(" ");
  Serial.print(data[1], HEX); Serial.print(" ");
  Serial.print(data[2], HEX); Serial.print(" ");
  Serial.println(data[3], HEX);
  */

  if ((data[0] == (~data[1] & 0xFF)) && (data[2] == (~data[3] & 0xFF))) {
    return data[2];
    
  }
  return -2;
}


uint16_t IRRemote::measurePulse(boolean state) {
  uint16_t pulse = 0;
  uint8_t statemask;
  
 if (state) {
   statemask = _irpinmask;
 } else {
   statemask = 0;
 }
 
 volatile uint8_t *_irpinreg = portInputRegister(_irpinport);
  // wait for the state to change
  //  while (((IRpin_PIN >> IRpin) & 0x1) != state) {
  while ((*_irpinreg & _irpinmask) != statemask) {
    delayMicroseconds(RESOLUTION);
    if (timing) {
      maxwaiting -= RESOLUTION;
      if (maxwaiting < 0) {
        timedout = true;
        return 0;
      }
    }
  }
  
  // in the proper state, keep track
  while ((*_irpinreg & _irpinmask) == statemask) {
     delayMicroseconds(RESOLUTION);
     pulse++;
     if (timing) {
      maxwaiting -= RESOLUTION;
      if (maxwaiting < 0) {
        timedout = true;
        return 0;
      }
    }
  }
  return pulse;
}


uint8_t IRRemote::readNECbit(void) {
  int16_t p1, p2;
  
  p1 = measurePulse(0);
  if (timedout) return -1;
  p2 = measurePulse(1); 
  if (timedout) return -1;

  if ((p1 < 400/RESOLUTION) || (p1 > 700/RESOLUTION)) 
    return -1;
  if ((p2 > 400/RESOLUTION) && (p2 < 700/RESOLUTION)) {
     return 0;   // valid bit
  }
   if ((p2 > 1400/RESOLUTION) && (p2 < 1800/RESOLUTION)) {
     return 1;   // valid bit
  } 
  return -1;
}
