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

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// what our timing resolution should be, larger is better
// as its more 'precise' - but too large and you wont get
// accurate timing
#define RESOLUTION 20 

class IRRemote {
 public:
  IRRemote(uint8_t pin);
  int16_t listen(int16_t maxwaitseconds = 0);

 private:
  uint8_t _irpin, _irpinport, _irpinmask;

  boolean timedout, timing;
  int32_t maxwaiting;

  uint16_t measurePulse(boolean state);
  uint8_t readNECbit();
};

