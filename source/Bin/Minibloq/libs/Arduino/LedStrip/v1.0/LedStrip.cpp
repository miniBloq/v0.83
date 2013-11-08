/*
  LedStrip library using simplifed protocol for 16MHz boards
  based on adafruit NeoPixel
 */ 

/*--------------------------------------------------------------------
  This file has some code derived from the  Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.
  --------------------------------------------------------------------*/

#include <LedStripConfig.h>
#include <LedStrip.h>

// #ifdef LedStripConfig_h
	// xxx#
// #endif


// test to verify board has 16MHz clock
#if (F_CPU <= 15400000UL) || (F_CPU > 19000000L)  
#error "code only supports 16MHz CPU"
#endif

//#define numbrOfPixels 8 //  <- should be defined in the hardware directory
//#define LED_STRIP_PIN 14 // <- As Above
// defines were tried in both of the following files located in directory: hard\Rovera\lib
#include <LedStripConfig.h> // neither of these work!
#include <board.h>

const uint16_t numBytes = numbrOfPixels *3;
uint8_t pixels[numBytes];

uint8_t _brightness = 255;
uint32_t _endTime;       // Latch timing reference

volatile uint8_t *dataPort;
uint8_t LED_STRIP_PINMask;


uint8_t stripR,stripG,stripB; // global color values
bool isLedStripStarted = false;

// Convert separate R,G,B into packed 32-bit RGB color.
// Packed format is always RGB, regardless of LED strand color order.
uint32_t convertColor(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g <<  8) | b;
}


void pixelSetBrightness(uint8_t b) {
  // Stored brightness value is different than what's passed.
  // This simplifies the actual scaling math later, allowing a fast
  // 8x8-bit multiply and taking the MSB.  'brightness' is a uint8_t,
  // adding 1 here may (intentionally) roll over...so 0 = max brightness
  // (color values are interpreted literally; no scaling), 1 = min
  // brightness (off), 255 = just below max brightness.
  uint8_t newBrightness = b + 1;
  if(newBrightness != _brightness) { // Compare against prior value
    // Brightness has changed -- re-scale existing data in RAM
    uint8_t  c,
            *ptr           = pixels,
             oldBrightness = _brightness - 1; // De-wrap old brightness value
    uint16_t scale;
    if(oldBrightness == 0) scale = 0; // Avoid /0
    else if(b == 255) scale = 65535 / oldBrightness;
    else scale = (((uint16_t)newBrightness << 8) - 1) / oldBrightness;
    for(uint16_t i=0; i<numBytes; i++) {
      c      = *ptr;
      *ptr++ = (c * scale) >> 8;
    }
    _brightness = newBrightness;
  }
}
void pixelBegin() {
  dataPort = portOutputRegister(digitalPinToPort(LED_STRIP_PIN));
  LED_STRIP_PINMask = digitalPinToBitMask(LED_STRIP_PIN); 
  pinMode(LED_STRIP_PIN, OUTPUT);
  digitalWrite(LED_STRIP_PIN, LOW);
}

// Set pixel color from separate R,G,B components:
void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if(n < numbrOfPixels) {
    if(_brightness) { // See notes in setBrightness()
      r = (r * _brightness) >> 8;
      g = (g * _brightness) >> 8;
      b = (b * _brightness) >> 8;
    }
    uint8_t *p = &pixels[n * 3];
    *p++ = g;
    *p++ = r;
    *p   = b;
  }
}
// Set pixel color from 'packed' 32-bit RGB color:
void setPixelColor(uint16_t n, uint32_t c) {
  if(n < numbrOfPixels) {
    uint8_t
      r = (uint8_t)(c >> 16),
      g = (uint8_t)(c >>  8),
      b = (uint8_t)c;
    if(_brightness) { // See notes in setBrightness()
      r = (r * _brightness) >> 8;
      g = (g * _brightness) >> 8;
      b = (b * _brightness) >> 8;
    }
    uint8_t *p = &pixels[n * 3];
    *p++ = g;
	*p++ = r; 
    *p = b;
  }
}

void showPixels() {
  while((micros() - _endTime) < 50L); // at least 50 microsecs between calls
  noInterrupts(); // Need 100% focus on instruction timing
  volatile uint16_t i   = numBytes; // Loop counter
  volatile uint8_t *ptr = pixels,   // Pointer to next byte
  b   = *ptr++,   // Current byte value
  hi,             // PORT w/output bit set high
  lo;             // PORT w/output bit set low

   // 40 inst. clocks per bit: HHHHHHHHxxxxxxxxxxxxLLLLLLLLLLLLLLLLLLLL
   // ST instructions:         ^       ^           ^         (T=0,8,20)

    volatile uint8_t next, bit;

    hi   = *dataPort |  LED_STRIP_PINMask;
    lo   = *dataPort & ~LED_STRIP_PINMask;
    next = lo;
    bit  = 8;

   asm volatile(
     "head20:\n\t"          // Clk  Pseudocode    (T =  0)
      "st   %a0, %1\n\t"    // 2    PORT = hi     (T =  2)
      "sbrc %2, 7\n\t"      // 1-2  if(b & 128)
       "mov  %4, %1\n\t"    // 0-1   next = hi    (T =  4)
      "dec  %3\n\t"         // 1    bit--         (T =  5)
      "st   %a0, %4\n\t"    // 2    PORT = next   (T =  7) ST and MOV don't
      "mov  %4, %5\n\t"     // 1    next = lo     (T =  8) change Z flag,
      "breq nextbyte20\n\t" // 1-2  if(bit == 0)       <-- so this is OK.
      "rol  %2\n\t"         // 1    b <<= 1       (T = 10)
      "rjmp .+0\n\t"        // 2    nop nop       (T = 12)
      "nop\n\t"             // 1    nop           (T = 13)
      "st   %a0, %5\n\t"    // 2    PORT = lo     (T = 15)
      "nop\n\t"             // 1    nop           (T = 16)
      "rjmp .+0\n\t"        // 2    nop nop       (T = 18)
      "rjmp head20\n\t"     // 2    -> head20 (next bit out)
     "nextbyte20:\n\t"      //                    (T = 10)
      "ldi  %3, 8\n\t"      // 1    bit = 8       (T = 11)
      "ld   %2, %a6+\n\t"   // 2    b = *ptr++    (T = 13)
      "st   %a0, %5\n\t"    // 2    PORT = lo     (T = 15)
      "nop\n\t"             // 1    nop           (T = 16)
      "sbiw %7, 1\n\t"      // 2    i--           (T = 18)
      "brne head20\n\t"     // 2    if(i != 0) -> head20 (next byte)
      ::
      "e" (dataPort),      // %a0
      "r" (hi),            // %1
      "r" (b),             // %2
      "r" (bit),           // %3
      "r" (next),          // %4
      "r" (lo),            // %5
      "e" (ptr),           // %a6
      "w" (i)              // %7
    ); // end asm

   interrupts();
   _endTime = micros(); // Save EOD time for latch on next call
}

// the next two functions are adafruit demos:
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t colorWheel(byte WheelPos) {
  if(WheelPos < 85) {
     return convertColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
     WheelPos -= 85;
     return convertColor(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
     WheelPos -= 170;
    return convertColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*2; j++) { // 2 cycles of all colors on wheel
    for(i=0; i< numbrOfPixels; i++) {
      setPixelColor(i, colorWheel(((i * 256 / numbrOfPixels) + j) & 255));
    }
    showPixels();
    delay(wait);
  }
}

// BarGraph functions

void BarGraphColor(uint8_t r, uint8_t g,uint8_t b)
{
   stripR = r;
   stripG = g;
   stripB = b;
}

void BarGraphBegin( ) 
{
  pixelBegin();
  //rainbowCycle(10);
  pixelSetBrightness(0);
  showPixels();
}

void BarGraph(uint32_t color, int percent )
{
  if(isLedStripStarted == false) {
      BarGraphBegin();
	  isLedStripStarted = true;
	  BarGraphColor( 0,128,0); // default color is green at half brightness
  }
  const byte MAX_BRIGHTNESS = 192;
  
  int brightness = MAX_BRIGHTNESS; // default brightness
  int percentPerPixel = 100/ numbrOfPixels;
  int count = percent / percentPerPixel;
  //Serial.print("LED count = "); Serial.print(count);
  if( count < numbrOfPixels-1) {
     brightness = percent % (100/ numbrOfPixels);
     brightness = map(brightness, 0,percentPerPixel, 0,MAX_BRIGHTNESS);
     //Serial.print(", brightness = "); Serial.println(brightness);
  } 
  pixelSetBrightness(200);
  stripR = (uint8_t)(color >> 16),
  stripG = (uint8_t)(color >>  8),
  stripB = (uint8_t)color;
  
  for(int i=0; i < numbrOfPixels; i++) {
      if(count > i)
        setPixelColor(i,color);
      else  if(i == count)  
         setPixelColor(i, map(stripR, 0,MAX_BRIGHTNESS,0,brightness),
                                map(stripG, 0,MAX_BRIGHTNESS,0,brightness),
                                map(stripB, 0,MAX_BRIGHTNESS,0,brightness)) ;
      else  
         setPixelColor(i, 0,0,0); 
  }
  showPixels(); 
}
