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

  
#ifndef SIMPLE_NEOPIXEL_H
#define SIMPLE_NEOPIXEL_H

// colors for LedStrip
#define WHITE 	0xFFFFFF	
#define GRAY 	0x808080	
#define MAROON 	0x800000	
#define RED 	0xFF0000	
#define OLIVE 	0x808000	
#define YELLOW 	0xFFFF00	
#define GREEN 	0x008000	
#define LIME 	0x00FF00	
#define TEAL 	0x008080	
#define AQUA 	0x00FFFF	
#define NAVY 	0x000080	
#define BLUE 	0x0000FF	
#define PURPLE 	0x800080	
#define FUCHSIA 0xFF00FF

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
 #include <pins_arduino.h>
#endif

extern void BarGraph(uint32_t color, int percent);

void pixelSetBrightness(uint8_t b);



#endif 
// SIMPLE_NEOPIXEL_H
