/*

LCDGraphics - Library for LCD Color 130x130 - V0.6
Author: Federico Lanza
ingfedericolanza [at] gmail [.] com

Some low level functions are based on LCD_driver.c by Mark Sproul
These functions were rewriten and rearranged in order to enter into the class schema

Changes:
  V0.4: April 2012	First release
  V0.5: April 2012	Added string, fonts, print, println capabilities
  V0.6: Nov 2012	Added support for new version of Print class.

*/

#ifndef LCDGRAPHICS_H
#define LCDGRAPHICS_H

#include "Arduino.h"
#include "fontdef.h"
#include "Print.h"
#include <stdint.h>

// Pin Definition for digitalWrite functions

#if defined(DUINOBOT_V1X) || defined(DUINOBOT_V1X_HID)

// D9 = CS  
#define LCD_CS_PIN	 9
// D8 = RES
// Note: LCD Reset PIN changed from Pin 8 to Pin 10
#define LCD_RES_PIN	10
// D11 = DIO
#define LCD_DIO_PIN	11
// D13 = SCK  
#define LCD_SCK_PIN	13

#else

// D9 = CS  
#define LCD_CS_PIN	 9
// D8 = RES
#define LCD_RES_PIN	 8
// D11 = DIO
#define LCD_DIO_PIN	11
// D13 = SCK  
#define LCD_SCK_PIN	13

#define _LCDGRAPHICS_HAL_ARDUINO_


#endif

// Colors
#define WHITE	0x000
#define BLACK	0xFFF
#define RED		0x0FF
#define	GREEN	0xF0F
#define BLUE	0xFF0
#define CYAN	0x0FF
#define MAGENTA	0xF0F
#define YELLOW	0xFF0
#define BROWN	0xB22
#define ORANGE	0xFA0
#define PINK	0xF6A 

// LCD Phillips Commands
#define	P_NOP			0x00	// nop 
#define	P_SWRESET		0x01	// software reset 
#define	P_BSTROFF		0x02	// booster voltage OFF 
#define	P_BSTRON		0x03	// booster voltage ON 
#define	P_RDDIDIF		0x04	// read display identification 
#define	P_RDDST			0x09	// read display status 
#define	P_SLEEPIN		0x10	// sleep in 
#define	P_SLEEPOUT		0x11	// sleep out 
#define	P_PTLON			0x12	// partial display mode 
#define	P_NORON			0x13	// display normal mode 
#define	P_INVOFF		0x20	// inversion OFF 
#define	P_INVON			0x21	// inversion ON 
#define	P_DALO			0x22	// all pixel OFF 
#define	P_DAL			0x23	// all pixel ON 
#define	P_SETCON		0x25	// write contrast 
#define	P_DISPOFF		0x28	// display OFF 
#define	P_DISPON		0x29	// display ON 
#define	P_CASET			0x2A	// column address set 
#define	P_PASET			0x2B	// page address set 
#define	P_RAMWR			0x2C	// memory write 
#define	P_RGBSET		0x2D	// colour set 
#define	P_PTLAR			0x30	// partial area 
#define	P_VSCRDEF		0x33	// vertical scrolling definition 
#define	P_TEOFF			0x34	// test mode 
#define	P_TEON			0x35	// test mode 
#define	P_MADCTL		0x36	// memory access control 
#define	P_SEP			0x37	// vertical scrolling start address 
#define	P_IDMOFF		0x38	// idle mode OFF 
#define	P_IDMON			0x39	// idle mode ON 
#define	P_COLMOD		0x3A	// interface pixel format 
#define	P_SETVOP		0xB0	// set Vop
#define	P_BRS			0xB4	// bottom row swap 
#define	P_TRS			0xB6	// top row swap 
#define	P_DISCTR		0xB9	// display control 
#define	P_DOR			0xBA	// data order 
#define	P_TCDFE			0xBD	// enable/disable DF temperature compensation 
#define	P_TCVOPE		0xBF	// enable/disable Vop temp comp 
#define	P_EC			0xC0	// internal or external oscillator 
#define	P_SETMUL		0xC2	// set multiplication factor 
#define	P_TCVOPAB		0xC3	// set TCVOP slopes A and B 
#define	P_TCVOPCD		0xC4	// set TCVOP slopes c and d 
#define	P_TCDF			0xC5	// set divider frequency 
#define	P_DF8COLOR		0xC6	// set divider frequency 8-color mode 
#define	P_SETBS			0xC7	// set bias system 
#define	P_RDTEMP		0xC8	// temperature read back 
#define	P_NLI			0xC9	// n-line inversion 
#define	P_RDID1			0xDA	// read ID1 
#define	P_RDID2			0xDB	// read ID2 
#define	P_RDID3			0xDC	// read ID3

// LCD Dimensions
#define LCDGRAPHICS_MIN_X          1
#define LCDGRAPHICS_MIN_Y          1
#define LCDGRAPHICS_MAX_X        130
#define LCDGRAPHICS_MAX_Y        130

// Uncomment the following line if you prefer to use digitalWrite funtion
// to handle LCD hardware. By using digitalWrite the LCD is slower.

#ifdef _LCDGRAPHICS_HAL_ARDUINO_


  #define LCD_SET_RES    digitalWrite(LCD_RES_PIN, HIGH)
  #define LCD_CLE_RES    digitalWrite(LCD_RES_PIN, LOW)
  
  #define LCD_SET_CS     digitalWrite(LCD_CS_PIN, HIGH)
  #define LCD_CLE_CS     digitalWrite(LCD_CS_PIN, LOW)
  
  #define LCD_SET_SCK    digitalWrite(LCD_SCK_PIN, HIGH)
  #define LCD_CLE_SCK    digitalWrite(LCD_SCK_PIN, LOW)
  
  #define LCD_SET_DIO    digitalWrite(LCD_DIO_PIN, HIGH)
  #define LCD_CLE_DIO    digitalWrite(LCD_DIO_PIN, LOW)
  
#else

#if defined(DUINOBOT_V1X) || defined(DUINOBOT_V1X_HID)

// Pins Macro definitions for Duinobot V1.x

  // RES = PB0
  #define LCD_SET_RES    (PORTB |= 0x01)
  #define LCD_CLE_RES    (PORTB &= 0xFE)

  // CS = PB7
  #define LCD_SET_CS     (PORTB |= 0x80)
  #define LCD_CLE_CS     (PORTB &= 0x7F)

  // DIO = PB2
  #define LCD_SET_DIO    (PORTB |= 0x04)
  #define LCD_CLE_DIO    (PORTB &= 0xFB)
  
  // SCK = PB1
  #define LCD_SET_SCK    (PORTB |= 0x02)
  #define LCD_CLE_SCK    (PORTB &= 0xFD)
  
#elif defined DUINOBOT_V2X

// Pins Macro definitions for Duinobot V2.x

  // RES = PC6
  #define LCD_SET_RES    (PORTC |= 0x40)
  #define LCD_CLE_RES    (PORTC &= 0xBF)

  // CS = PD6
  #define LCD_SET_CS     (PORTD |= 0x40)
  #define LCD_CLE_CS     (PORTD &= 0xBF)

  // DIO = PB5
  #define LCD_SET_DIO    (PORTB |= 0x20)
  #define LCD_CLE_DIO    (PORTB &= 0xDF)
  
  // SCK = PB7
  #define LCD_SET_SCK    (PORTB |= 0x80)
  #define LCD_CLE_SCK    (PORTB &= 0x7F)

#endif
    
#endif

#define LCDGRAPHICS_INVERT_X(Val)  ((LCDGRAPHICS_MAX_X + 1) - Val)

class LCDGraphics : public Print
{
  private:
    // Font properties
    uint8_t *CharmapTable;    // Pointer to Bitmap font descriptor table
    struct CharDescriptor *CharDescrTable;    // Pointer to font descriptor data table
    uint8_t FontHeight;    // Font heigth in use
    // Private methods for writing data to LCD    
    void SendData (uint8_t Data);
    void SendCommand (uint8_t Cmd);
    void SendByte (uint8_t Data);
    // methods for Fonts management
    uint8_t GetCharIndex (char Char);
    void AddSpace (uint8_t x, uint8_t y, uint8_t Count);
    // overriden methods from Print class
	#if defined(DUINOBOT_V1X) || defined(DUINOBOT_V1X_HID)
    void write(uint8_t Char);
    void write(const char *str);
	#else
	size_t write(uint8_t Char);
	#endif
  public:
    // Colors and Coordinates
    uint16_t Foreground;    // Foreground Colour
    uint16_t Background;    // Background Color
    uint8_t CurrentX, CurrentY;    // Current coordinates
    //  public methods
    LCDGraphics ();
    LCDGraphics (uint16_t ForeColor, uint16_t BackColor);
    void Clear ();
    void SetFont (uint8_t *Bitmaps, struct CharDescriptor *Descriptors, uint8_t Height);
    void PutPixel (uint8_t x, uint8_t y, uint16_t Color);
    void DrawLine (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    void DrawRectangle (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    void DrawCircle (uint8_t CenterX, uint8_t CenterY, uint8_t Radius);
    void GotoXY (uint8_t x, uint8_t y);
};

#endif
