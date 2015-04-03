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

#include "LCDGraphics.h"

/*
Constructor LCDGraphics()
Initializes LCD Graphics hardware
*/
LCDGraphics::LCDGraphics ()
{
  // LCD Pins
  // ##
  pinMode(LCD_CS_PIN, OUTPUT);
  pinMode(LCD_DIO_PIN, OUTPUT);
  pinMode(LCD_SCK_PIN, OUTPUT);
  pinMode(LCD_RES_PIN, OUTPUT);

  // Init Commands
  // Reset
  LCD_CLE_RES;
  LCD_SET_RES;

  // SleepOut
  // Enable
  LCD_CLE_CS;

  SendCommand(P_SLEEPOUT);
  // Invert Mode
  SendCommand(P_INVON);
  // Color Mode
  SendCommand(P_COLMOD);
  SendData(0x03);
  // MAC
  SendCommand(P_MADCTL);
  SendData(0xC0);
  // Set Contrast
  SendCommand(P_SETCON);
  SendData(0x30);
  // Display ON
  SendCommand(P_DISPON);

  // Disable
  LCD_SET_CS;
}

/*
Constructor LCDGraphics(int16_t BackColor)
Initializes LCD Graphics hardware and clear screen with NackColor
Background and Foreground colors are set
Coordinates are initialized both to 1
*/
LCDGraphics::LCDGraphics (uint16_t ForeColor, uint16_t BackColor)
{
  // Calls hardware initializer
  LCDGraphics();
  // Initializes Colors and coordinates
  Foreground = ForeColor;
  Background = BackColor;
  CurrentX = CurrentY = 1;
  // Clear screen with Background color definition
  Clear();
}

/*
Clears all display content
Background color is current defined Background Color
*/
void LCDGraphics::Clear ()
{
  uint16_t i;

  // Enable
  LCD_CLE_CS;
  
  // Display OFF
  SendCommand(P_DISPOFF);

  SendCommand(P_PASET);
  SendData(LCDGRAPHICS_MIN_Y);
  SendData(LCDGRAPHICS_MAX_Y);

  SendCommand(P_CASET);
  SendData(LCDGRAPHICS_MIN_X);
  SendData(LCDGRAPHICS_MAX_X);
  SendCommand(P_RAMWR);
  
  for (i = 0; i < 8712; i++)
  {
    SendData((Background >> 4) & 0x00FF);
    SendData(((Background & 0x0F) << 4) | (Background >> 8));
    // NOP Epson
    SendData(Background & 0x0FF);
  }

  // Display ON
  SendCommand(P_DISPON);
  
  // Disable
  LCD_SET_CS;
}

/*
Defines current font to be used
*/
void LCDGraphics::SetFont (uint8_t *Bitmaps, struct CharDescriptor *Descriptors, uint8_t Height)
{
  CharmapTable = Bitmaps;
  CharDescrTable = Descriptors;
  FontHeight = Height;
}

/*
Sets pixel in x,y with Color
*/
void LCDGraphics::PutPixel (uint8_t x, uint8_t y, uint16_t Color)
{
  // Invert X coordinate for LCD invertion issue
  x = LCDGRAPHICS_INVERT_X(x);

  // Enable
  LCD_CLE_CS;

  // Page (row) start/end ram
  SendCommand(P_PASET);
  SendData(x);
  SendData(x);
  
  // Column start/end ram
  SendCommand(P_CASET);	
  SendData(y);
  SendData(y);
  
  // Write
  SendCommand(P_RAMWR);

  SendData((uint8_t)(Color >> 4));
  SendData((uint8_t)((Color & 0x0F) << 4));

  // Disable
  LCD_SET_CS;
}

/*
Draws a line from coordinate (x1, y1) to coordinate (x2, y2)
Color is current Foreground color
*/
void LCDGraphics::DrawLine (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  #define sgn(x) ((x < 0) ? -1 : ((x > 0) ? 1 : 0))
  int16_t n, sgndeltax, sgndeltay, deltaxabs, deltayabs, deltax, deltay, x, y, drawx, drawy;

  deltax = x2 - x1; 
  deltay = y2 - y1; 
  deltaxabs = abs(deltax); 
  deltayabs = abs(deltay); 
  sgndeltax = sgn(deltax); 
  sgndeltay = sgn(deltay); 
  x = deltayabs >> 1; 
  y = deltaxabs >> 1; 
  drawx = x1; 
  drawy = y1; 

  PutPixel(drawx, drawy, Foreground); 

  if (deltaxabs >= deltayabs)
  {
    for (n = 0; n < deltaxabs; n++)
    {
      y += deltayabs;
      if (y >= deltaxabs)
      {
        y -= deltaxabs;
        drawy += sgndeltay;
      }
      drawx += sgndeltax;
      PutPixel(drawx, drawy, Foreground);
    }
  }
  else
  {
    for (n = 0; n < deltayabs; n++)
    {
      x += deltaxabs;
      if (x >= deltayabs)
      {
        x -= deltayabs;
        drawx += sgndeltax;
      }
      drawy += sgndeltay;
      PutPixel(drawx, drawy, Foreground);
    }
  }
}

/*
Draws a Rectangle defined by coordinates (x1, y1) and (x2, y2)
Color is current Foreground color
*/
void LCDGraphics::DrawRectangle (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  DrawLine (x1, y1, x2, y1);
  DrawLine (x2, y1, x2, y2);
  DrawLine (x1, y2, x2, y2);
  DrawLine (x1, y1, x1, y2);
}

/*
Draws a Circle with center in (x, y) coordinates and with Radius as its radius
Color is current Foreground color
*/
void LCDGraphics::DrawCircle (uint8_t CenterX, uint8_t CenterY, uint8_t Radius)
{
  int8_t x, y;
  int16_t p;

  x = 0;
  y = Radius;
  PutPixel(CenterX, CenterY - Radius, Foreground);

  p = 3 - (2 * Radius);
  for (x = 0; x <= y; x++)
  {
    if (p < 0)
    {
      p = (p + (4*x) + 6);
    }
    else
    {
      y = y - 1;
      p = p + ((4 * (x-y) + 10));
    }
    PutPixel(CenterX + x, CenterY - y, Foreground);
    PutPixel(CenterX - x, CenterY - y, Foreground);
    PutPixel(CenterX + x, CenterY + y, Foreground);
    PutPixel(CenterX - x, CenterY + y, Foreground);
    PutPixel(CenterX + y, CenterY - x, Foreground);
    PutPixel(CenterX - y, CenterY - x, Foreground);
    PutPixel(CenterX + y, CenterY + x, Foreground);
    PutPixel(CenterX - y, CenterY + x, Foreground);
  }
}

void LCDGraphics::GotoXY (uint8_t x, uint8_t y)
{
  CurrentX = x;
  CurrentY = y;
}

/************************************
Private Methods
*************************************/

uint8_t LCDGraphics::GetCharIndex (char Char)
{
  return (Char - '!');
}

void LCDGraphics::AddSpace (uint8_t x, uint8_t y, uint8_t Count)
{
  uint8_t j, i;
  
  for (i = 0; i < Count; i++)
  {
    for (j = 0; j < FontHeight; j++)
    {
      PutPixel(x + i, y + j, Background);
    }
  }
  CurrentX += Count;
}

/*
Overriden method for printing a char
It will be printed in current coordinates and with current defined colors
*/

#if defined(DUINOBOT_V1X) || defined(DUINOBOT_V1X_HID)

void LCDGraphics::write(uint8_t Char)
{
  uint8_t j, jx, i, w, Ix;
  uint16_t St;
  uint8_t MapData1, MapData2;
  uint16_t Colour;

  // CR+LF
  if (Char == '\r')
  {
    CurrentX = 1;
    return;
  }
  if (Char == '\n')
  {
    CurrentY += (FontHeight + 1);
    return;
  }
  // SP
  if (Char == ' ')
  {
    AddSpace(CurrentX, CurrentY, 2);
    return;
  }
  // Normal char: get descriptors and put pixels accordingly
  Ix = GetCharIndex(Char);
  w = CharDescrTable[Ix].Wide;
  St = CharDescrTable[Ix].CharmapStart;
  if (w > 8)
  {
    j = 0; jx = 0;
    while (j < FontHeight)
    {
      MapData1 = CharmapTable[St + jx];
      MapData2 = CharmapTable[St + jx + 1];
      for (i = 0; i < 8; i++)
      {
        Colour = (MapData1 & 0x80) ? Foreground : Background;
        PutPixel(CurrentX + i, CurrentY + j, Colour);
        MapData1 <<= 1;
      }
      for (i = 0; i < (w - 8); i++)
      {
        Colour = (MapData2 & 0x80) ? Foreground : Background;
        PutPixel(CurrentX + i + 8, CurrentY + j, Colour);
        MapData2 <<= 1;
      }
      jx += 2;
      j++;
    }
  }
  else
  {
    for (j = 0; j < FontHeight; j++)
    {
      MapData1 = CharmapTable[St + j];
      for (i = 0; i < w; i++)
      {
        Colour = (MapData1 & 0x80) ? Foreground : Background;
        PutPixel(CurrentX + i, CurrentY + j, Colour);
        MapData1 <<= 1;
      }
    }
  }
  CurrentX += w;
  AddSpace(CurrentX, CurrentY, 1);
}

/*
Overriden method for printing a literal string
It will be printed in current coordinates and with current defined colors
*/
void LCDGraphics::write(const char *str)
{
  while (*str)
  {
    if (*str == ' ')
    {
      AddSpace(CurrentX, CurrentY, 2);
    }
    else
    {
      write(*str);
    }
    str++;
  }
}

#else

size_t LCDGraphics::write(uint8_t Char)
{
  uint8_t j, jx, i, w, Ix;
  uint16_t St;
  uint8_t MapData1, MapData2;
  uint16_t Colour;

  // CR+LF
  if (Char == '\r')
  {
    CurrentX = 1;
    return 1;
  }
  if (Char == '\n')
  {
    CurrentY += (FontHeight + 1);
    return 1;
  }
  // SP
  if (Char == ' ')
  {
    AddSpace(CurrentX, CurrentY, 2);
    return 1;
  }
  // Normal char: get descriptors and put pixels accordingly
  Ix = GetCharIndex(Char);
  w = CharDescrTable[Ix].Wide;
  St = CharDescrTable[Ix].CharmapStart;
  if (w > 8)
  {
    j = 0; jx = 0;
    while (j < FontHeight)
    {
      MapData1 = CharmapTable[St + jx];
      MapData2 = CharmapTable[St + jx + 1];
      for (i = 0; i < 8; i++)
      {
        Colour = (MapData1 & 0x80) ? Foreground : Background;
        PutPixel(CurrentX + i, CurrentY + j, Colour);
        MapData1 <<= 1;
      }
      for (i = 0; i < (w - 8); i++)
      {
        Colour = (MapData2 & 0x80) ? Foreground : Background;
        PutPixel(CurrentX + i + 8, CurrentY + j, Colour);
        MapData2 <<= 1;
      }
      jx += 2;
      j++;
    }
  }
  else
  {
    for (j = 0; j < FontHeight; j++)
    {
      MapData1 = CharmapTable[St + j];
      for (i = 0; i < w; i++)
      {
        Colour = (MapData1 & 0x80) ? Foreground : Background;
        PutPixel(CurrentX + i, CurrentY + j, Colour);
        MapData1 <<= 1;
      }
    }
  }
  CurrentX += w;
  AddSpace(CurrentX, CurrentY, 1);
  return 1;
}

#endif

/*
Sends a byte to LCD
*/
void LCDGraphics::SendByte (uint8_t Data)
{
  uint8_t j;
  
  for (j = 0; j < 8; j++)
  {
    LCD_CLE_SCK;
    if ((Data & 0x80))
      LCD_SET_DIO;
    else
      LCD_CLE_DIO;
    Data <<= 1;
    LCD_SET_SCK;
    LCD_CLE_SCK;
  }
}

/*
Sends a Command to LCD
*/
void LCDGraphics::SendCommand (uint8_t Cmd)
{
  // Send clock pulse
  LCD_CLE_SCK;
  // Output low on data out (9th bit low = command), p0.19
  LCD_CLE_DIO;
  // Send Pulse
  LCD_SET_SCK;
  // Send data
  SendByte(Cmd);
}

/*
Sends a Data byte to LCD
*/
void LCDGraphics::SendData (uint8_t Data)
{
  // Send clock pulse
  LCD_CLE_SCK;
  // Output high on data out (9th bit high = data)
  LCD_SET_DIO;
  // Send Pulse
  LCD_SET_SCK;
  // Send data
  SendByte(Data);
}
