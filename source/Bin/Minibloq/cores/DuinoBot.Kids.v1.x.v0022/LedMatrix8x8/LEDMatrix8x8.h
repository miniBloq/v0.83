#ifndef LEDMatrix8x8__h
#define LEDMatrix8x8__h

/* ==========================================================================
    ##NOTE: This library is intended for a very fast use so the digitalWrite
    functions are not used.
   ==========================================================================*/
#include <avr/io.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "LEDMatrix8x8Pins.h"
#include "LEDMatrix8x8Font.h"
#include "LEDMatrix8x8Sprites.h"

#include "WString.h"
#include "Print.h"
#include "WProgram.h"


const int screenUp = 1;
const int screenDown = 0;
const int screenInverted = 0;
const int screenNormal = 1;

//const int DEC = 10;
//const int HEX = 16;
//const int OCT = 8;
//const int BIN = 2;
//const int BYTE = 0;

class LEDMatrix8x8
{
	private:
        static const uint8_t memSize = 8;
        static const uint8_t bufferSize = 60;
        static const uint8_t animationDelay = 200;
		
        int textScrollDelay;
        int animationScrollDelay;

		uint8_t screenMem[memSize];     // screen memory
		uint8_t activeRow;              // active row
		uint8_t screenOrientation;      // Defines orientation 0:down, 1: up
		uint8_t	_screenInverted;

        //##Definir este "60" en algún lado:
		uint8_t buffer[bufferSize];     // stores the active message or sprite
		uint8_t messagePtr;             // points to the active char in the message
		uint8_t messageDisplayed;       // how often has the message been displayed?
		uint8_t activeChar;             // stores the active char
		uint8_t messageLength;          // stores the length of the active message
		uint8_t charPtr;                // points to the active col in the char
		uint8_t charLength;             // stores the length of the active char

		inline void copyToBuffer(const prog_uint8_t sprite[])
        {
            memcpy_P(buffer, sprite, memSize);
        }
		void swapBuffers(const int8_t x, const int8_t y, const uint8_t sprite[]);
		void showNextBufferChar();
		
		// This functions scrolls.
		void printLong(long i, const int radix = 10);
		void printDouble(double f, const int prec = 4);
		
		volatile uint8_t *col1_PORT;
		volatile uint8_t *col2_PORT;
		volatile uint8_t *col3_PORT;
		volatile uint8_t *col4_PORT;
		volatile uint8_t *col5_PORT;
		volatile uint8_t *col6_PORT;
		volatile uint8_t *col7_PORT;
		volatile uint8_t *col8_PORT;
		
		volatile uint8_t col1_PIN; 
		volatile uint8_t col2_PIN; 
		volatile uint8_t col3_PIN; 
		volatile uint8_t col4_PIN; 
		volatile uint8_t col5_PIN; 
		volatile uint8_t col6_PIN; 
		volatile uint8_t col7_PIN; 
		volatile uint8_t col8_PIN; 
							
		volatile uint8_t * row_mux_0_PORT;
		volatile uint8_t * row_mux_1_PORT;
		volatile uint8_t * row_mux_2_PORT;
		
		volatile uint8_t row_mux_0_PIN;
		volatile uint8_t row_mux_1_PIN;
		volatile uint8_t row_mux_2_PIN;

		uint8_t	barValue[4];
		
    public:
		LEDMatrix8x8( const int col1,
					  const int col2,
					  const int col3,
					  const int col4,
					  const int col5,
					  const int col6,
					  const int col7,
					  const int col8,
					  const int row1,
					  const int row2,
					  const int row3,
					  const int textScrollDelay = 100,
                      const int animationScrollDelay = 80,
                      const uint8_t screenOrientation = screenUp,
                      const uint8_t _screenInverted = screenNormal
                     );
		inline ~LEDMatrix8x8()
		{
		}

		void refresh();
		void clearScreen();

        inline void setPixel(const uint8_t x, const uint8_t y)
        {
            //##Eliminate magic numbers:
            screenMem[y] |= (128>>x);
        }

        inline void clearPixel(const uint8_t x, const uint8_t y)
        {
            //##Eliminate magic numbers:
            screenMem[y] &= ~(128>>x);
        }

        inline void setTextScrollDelay(const int value)
        {
            textScrollDelay = value;
        }

        inline int getTextScrollDelay() const
        {
            return textScrollDelay;
        }

        inline void setAnimationScrollDelay(const int value)
        {
            animationScrollDelay = value;
        }

        inline int getAnimationScrollDelay() const
        {
            return animationScrollDelay;
        }
        
        inline void setOrientation(const uint8_t orient)
        {
			screenOrientation = orient;
        }
        
        inline uint8_t getOrientation() const
        {
        	return screenOrientation;
        }

		inline void setInverted(const uint8_t inv){
			_screenInverted = inv;
		}
		
		inline uint8_t getInverted() const
		{
			return _screenInverted;
		}
		
		// This functions doesn't scroll, shows the single char
		void putChar(const char c,uint8_t x_offset=1, uint8_t y_offset=2, uint8_t clear=1);
		void putNumber(const int number);
		
		// Screen scroll
		void scrollLeft(const uint8_t x);
		void scrollRight(const uint8_t x);
		void scrollUp(const uint8_t x);
		void scrollDown(const uint8_t x);
	
		// There is no inherit because we can't implement write(char)

		//inline void print(const String & s) ;
		void print(const char[]);
		inline void print(char c, int base = BYTE) {this->putChar(c);}
		inline void print(unsigned char c, int base= BYTE) {this->putChar(c);}
		inline void print(int i, int base = DEC) {this->printLong((long)i, base);}
		inline void print(unsigned int i, int base = DEC) {this->printLong((long)i, base);}
		inline void print(long l, int base = DEC) {this->printLong(l, base);}
		inline void print(unsigned long l, int base = DEC) {this->printLong(l, base);}
		inline void print(double d, int prec = 2) {this->printDouble(d, prec);}

		void showBars();
		void showAllBars(const int bar0, const int bar1, const int bar2, const int bar3, const int lower, const int upper);

		inline void setBar(const uint8_t bar, const uint8_t value){
			barValue[bar] = value;
		}
		
		inline uint8_t getBar(const uint8_t bar){
			return barValue[bar];
		}

		//##Hace falta el [8]? Ver el tema puntero
		void showSprite(const prog_uint8_t sprite[]);
		void scrollAnimation(const prog_uint8_t sprite_1[], const prog_uint8_t sprite_2[]);
};

extern LEDMatrix8x8 Screen;
#endif
