#include "LEDMatrix8x8.h"
#include "wiring.h"
#include "pins_arduino.h"
#include "../pins.h"

LEDMatrix8x8::LEDMatrix8x8( const int col8,
					  		const int col7,
					  		const int col6,
						  	const int col5,
						  	const int col4,
						  	const int col3,
						  	const int col2,
						  	const int col1,
						  	const int row0,
						  	const int row1,
						  	const int row2,
							const int textScrollDelay,
                            const int animationScrollDelay,
                            const uint8_t screenOrientation,
                            const uint8_t screenInverted)  	 :  textScrollDelay(textScrollDelay),
                                                                animationScrollDelay(animationScrollDelay),
                                                                messagePtr(0),
                                                                messageDisplayed(0),
                                                                activeChar(0),
                                                                messageLength(0),
                                                                charPtr(0),
                                                                charLength(0),
                                                                screenOrientation(screenOrientation),
                                                                _screenInverted(screenInverted)
                                                                
{

	pinMode(col1,OUTPUT);
	pinMode(col2,OUTPUT);
	pinMode(col3,OUTPUT);
	pinMode(col4,OUTPUT);
	pinMode(col5,OUTPUT);
	pinMode(col6,OUTPUT);
	pinMode(col7,OUTPUT);
	pinMode(col8,OUTPUT);
	
	pinMode(row0,OUTPUT);
	pinMode(row1,OUTPUT);
	pinMode(row2,OUTPUT);
	
	digitalWrite(col1,LOW);
	digitalWrite(col2,LOW);
	digitalWrite(col3,LOW);
	digitalWrite(col4,LOW);
	digitalWrite(col5,LOW);
	digitalWrite(col6,LOW);
	digitalWrite(col7,LOW);
	digitalWrite(row0,LOW);
	digitalWrite(row1,LOW);
	digitalWrite(row2,LOW);	

	col1_PORT = portOutputRegister(digitalPinToPort(col1));
	col1_PIN = digitalPinToBitMask(col1);
	col2_PORT = portOutputRegister(digitalPinToPort(col2));
	col2_PIN = digitalPinToBitMask(col2);
	col3_PORT = portOutputRegister(digitalPinToPort(col3));
	col3_PIN = digitalPinToBitMask(col3);
	col4_PORT = portOutputRegister(digitalPinToPort(col4));
	col4_PIN = digitalPinToBitMask(col4);
	col5_PORT = portOutputRegister(digitalPinToPort(col5));
	col5_PIN = digitalPinToBitMask(col5);
	col6_PORT = portOutputRegister(digitalPinToPort(col6));
	col6_PIN = digitalPinToBitMask(col6);
	col7_PORT = portOutputRegister(digitalPinToPort(col7));
	col7_PIN = digitalPinToBitMask(col7);
	col8_PORT = portOutputRegister(digitalPinToPort(col8));
	col8_PIN = digitalPinToBitMask(col8);

	row_mux_0_PORT = portOutputRegister(digitalPinToPort(row0));
	row_mux_0_PIN = digitalPinToBitMask(row0);
	row_mux_1_PORT = portOutputRegister(digitalPinToPort(row1));
	row_mux_1_PIN = digitalPinToBitMask(row1);
	row_mux_2_PORT = portOutputRegister(digitalPinToPort(row2));
	row_mux_2_PIN = digitalPinToBitMask(row2);
	
	barValue[0]=0;
	barValue[1]=0;
	barValue[2]=0;		
	barValue[3]=0;		
}


//==========================================================
//						Primitives
//==========================================================
void LEDMatrix8x8::clearScreen()
{
    uint8_t i;
    for (i = 0; i < memSize; i++)
        screenMem[i] = 0x00;
}


//==========================================================
//						Screen Movement
//==========================================================
void LEDMatrix8x8::scrollLeft(const uint8_t x)
{
  //Blit the screen to the left
  for (int i = 0; i < memSize; i++)
    screenMem[i] <<= x;
}


void LEDMatrix8x8::scrollRight(const uint8_t x)
{
	//Blit the screen to the left
	for (int i = 0; i < memSize; i++)
		screenMem[i] >>= x;
}

void LEDMatrix8x8::scrollUp(const uint8_t x)
{

	for (int i=0;i<memSize;i++){
		if ((i-x)>=0){
			screenMem[i-x]=screenMem[i];
		}
	}
}

void LEDMatrix8x8::scrollDown(const uint8_t x)
{

	for (int i=memSize;i>0;i--){
		if ((i+x)<=7){
			screenMem[i+x]=screenMem[i];
		}
	}
}

//==========================================================
//							Text
//==========================================================


void LEDMatrix8x8::putChar(char c, uint8_t x_offset,  uint8_t y_offset, uint8_t clear)
{
	uint8_t i,j;
	uint8_t b;

	uint8_t charLen;


	if (clear==1)
		this->clearScreen();

	if ((c>='a') && (c<='z')){
		c = c-('a'-'A');
	}else if ((c>'z')||((c<32))){
		//invalid
		c=95;
	}
	
	c = c-CHAR_OFFSET;
		
	charLen = pgm_read_byte(&font[c * 4 + 3]);

	// read pixels for current column of char
		
	for (i=0;i<charLen;i++)
	{
		b = pgm_read_byte(&font[c * 4 + i]);

		for (j = 0; j < (memSize )-y_offset; j++)
		{
			if ((b & (1 << j)) == (1 << j))
				screenMem[j+y_offset] |= ((0x01<<((charLen+x_offset)-i)));
		}
    
	}
}

void LEDMatrix8x8::putNumber(const int number){

	char digit1, digit2;

	if (number>99)
	{
		digit1 = '+';
		digit2 = '+';
		
		this->putChar(digit2,-1,1);
		this->putChar(digit1,3,1,0);
		
	}
	else if (number<-99)
	{
		digit1 = '-';
		digit2 = '-';
		
		this->putChar(digit2,-1,2);
		this->putChar(digit1,3,2,0);
		
	}
	else
	{
		digit1 = abs((number/10))+'0';
		digit2 = abs((number%10))+'0';
		
		this->putChar(digit2,-1,3);
		this->putChar(digit1,3,3,0);
		
		screenMem[1]|=0x1C;
		if (number>0){
			screenMem[0]|=0x08;
			screenMem[2]|=0x08;
		}
	}
	
	
}

void LEDMatrix8x8::print(const char * s)
{
	uint8_t repeat = 1;

	//This is due to the +1 increment, otherwise a blank space should be put before:
	buffer[0] = ' ';

    //20100915: strcpy copies all, including the NULL, and does not perform any error check:
	//strcpy((char*)buffer + 1, s);
	strlcpy((char*)buffer + 1, s, sizeof(buffer) - 1);
	messageLength = strlen((char*)buffer);

	while (messageDisplayed < repeat)
	{
		showNextBufferChar();
		delay(textScrollDelay);
	}

	messageDisplayed = 0;
}


void LEDMatrix8x8::printLong( long i, const int radix)
{
	char	str[50];
	ltoa(i,str,radix);
	this->print(str);
	
}

void LEDMatrix8x8::printDouble( double f, const int prec)
{
	char	str[50];
	dtostrf(f, 6, prec, str);
	this->print(str);
	
}

void LEDMatrix8x8::showNextBufferChar()
{
    uint8_t i;
    uint8_t b;

    //Blit the screen to the left
    for (i = 0; i < memSize; i++)
        screenMem[i] <<= 1;

    //Advance a char if needed
    if (charPtr == charLength)
    {
        messagePtr++;
        if (messagePtr == messageLength)
        {
            messagePtr = 0;
            messageDisplayed++;
        }
        
        // Check the interval and strtoUpper de lowercase characters
		if ((buffer[messagePtr] >='a') && (buffer[messagePtr] <='z')){
			buffer[messagePtr]  = buffer[messagePtr] -('a'-'A');
		}else if ((buffer[messagePtr] >'z')||((buffer[messagePtr] <32))){
			//invalid
			buffer[messagePtr] =95;
		}
	
        activeChar = buffer[messagePtr] - CHAR_OFFSET;
        charLength = pgm_read_byte(&font[activeChar * 4 + 3]); //##Eliminate magic numbers
        charPtr = 0;
        return; //This makes the space between two chars
    }

    //Read pixels for current column of char
    b = pgm_read_byte(&font[activeChar * 4 + charPtr++]); //##Eliminate magic numbers

    //Write pixels into screen memory
    for (i = 0; i < (memSize - 1); i++)
    {
        if ((b & (1 << i)) == (1 << i))
            screenMem[i+1] |= 0x01;
    }
}

//==========================================================
//					Data
//==========================================================

void LEDMatrix8x8::showBars()
{
//	int tmp;
	
	this->clearScreen();
	
	for (uint8_t i=0;i<4;i++)
	{ 
		for (uint8_t y=0;y<memSize;y++)
		{
			if (y<barValue[i])
			{
		    	screenMem[(memSize-1)-y] |= (0b11000000)>>(2*i);
		    }
	    }
	}
}

void LEDMatrix8x8::showAllBars(const int bar0, const int bar1, const int bar2, const int bar3, const int lower, const int upper){
	this->setBar(0,map(bar0,lower,upper,0,8));
	this->setBar(1,map(bar1,lower,upper,0,8));
	this->setBar(2,map(bar2,lower,upper,0,8));
	this->setBar(3,map(bar3,lower,upper,0,8));	
	this->showBars();
}
//==========================================================
//					Images/Sprites
//==========================================================

void LEDMatrix8x8::showSprite(const prog_uint8_t sprite[])
{
    copyToBuffer(sprite);
    swapBuffers(0, 0, buffer);
}


void LEDMatrix8x8::swapBuffers(const int8_t x, const int8_t y, const uint8_t sprite[])
{
    int8_t i, t;
    uint8_t row;
    for (i = 0; i < memSize; i++)
    {
        t = i - y;
        row = ((t >= 0) && (t < memSize)) ? sprite[t] : 0x00;
        row = (x >= 0) ? (row >> x) : (row << -x);
        screenMem[i] = row;
    }
}


void LEDMatrix8x8::scrollAnimation(const prog_uint8_t sprite_1[], const prog_uint8_t sprite_2[])
{
    uint8_t i;
	int8_t x;

	copyToBuffer(sprite_1);

	//##Eliminate magic numbers ("8" in this case):
	for (x = -8; x <= 0; x++)
	{
        swapBuffers(x, 0, buffer);
        delay(animationScrollDelay);
	}
	delay(animationDelay);
	copyToBuffer(sprite_2);
	swapBuffers(0, 0, buffer);
	delay(animationDelay);
	copyToBuffer(sprite_1);
	swapBuffers(0, 0, buffer);
	delay(animationDelay);
	copyToBuffer(sprite_2);
	swapBuffers(0, 0, buffer);
	delay(animationDelay);
	copyToBuffer(sprite_1);

    //##Eliminate magic numbers:
	for (x = 0; x < 9; x++)
	{
        swapBuffers(x, 0, buffer);
        delay(animationScrollDelay);
	}
}


//==========================================================
//						Low Level
//==========================================================
void LEDMatrix8x8::refresh()
{	
	// TODO: This functions MUST BE optimized
    uint8_t row;

    //Shut down all rows and columns
    *col1_PORT &= ~col1_PIN;
    *col2_PORT &= ~col2_PIN;
    *col3_PORT &= ~col3_PIN;
    *col4_PORT &= ~col4_PIN;
    *col5_PORT &= ~col5_PIN;
    *col6_PORT &= ~col6_PIN;
    *col7_PORT &= ~col7_PIN;
    *col8_PORT &= ~col8_PIN;

    //Next row
    activeRow = (activeRow+1) % memSize;
    
	if (screenOrientation==0)
	{
    	row = screenMem[activeRow];
	}
	else if (screenOrientation==1)
	{
		row = screenMem[(memSize-1) - activeRow];
	}
	
	if (_screenInverted == screenInverted)
		row = ~row;
    //Output all columns, switch leds on.

    //Column 1
    if ((row & 0x80) == 0x80)
    {
    	if (screenOrientation==0)
        	*col1_PORT |= col1_PIN;
        else
        	*col8_PORT |= col8_PIN;
    }
    //Column 2
    if ((row & 0x40) == 0x40)
    {
    	if (screenOrientation==0)
        	*col2_PORT |= col2_PIN;
        else
        	*col7_PORT |= col7_PIN;
    }
    //Column 3
    if ((row & 0x20) == 0x20)
    {
    	if (screenOrientation==0)
        	*col3_PORT |= col3_PIN;
        else
        	*col6_PORT |= col6_PIN;
    }
    //Column 4
    if ((row & 0x10) == 0x10)
    {
    	if (screenOrientation==0)
        	*col4_PORT |= col4_PIN;
        else
        	*col5_PORT |= col5_PIN;
    }
    //Column 5
    if ((row & 0x08) == 0x08)
    {
    	if (screenOrientation==0)
        	*col5_PORT |= col5_PIN;
        else
        	*col4_PORT |= col4_PIN;
    }
    //Column 6
    if ((row & 0x04) == 0x04)
    {
    	if (screenOrientation==0)
        	*col6_PORT |= col6_PIN;
        else
        	*col3_PORT |= col3_PIN;
    }
    //Column 7
    if ((row & 0x02) == 0x02)
    {
    	if (screenOrientation==0)
        	*col7_PORT |= col7_PIN;
        else
        	*col2_PORT |= col2_PIN;
    }
    //Column 8
    if ((row & 0x01) == 0x01)
    {
    	if (screenOrientation==0)
        	*col8_PORT |= col8_PIN;
        else
        	*col1_PORT |= col1_PIN;
    }


	if (activeRow & 0x01) {
		*row_mux_0_PORT |= row_mux_0_PIN;
	}else{
		*row_mux_0_PORT &=~row_mux_0_PIN;
	}
	
	if (activeRow & 0x02) {
		*row_mux_1_PORT |= row_mux_1_PIN;
	}else{
		*row_mux_1_PORT &=~row_mux_1_PIN;
	}
	
	if (activeRow & 0x04) {
		*row_mux_2_PORT |= row_mux_2_PIN;
	}else{
		*row_mux_2_PORT &=~row_mux_2_PIN;
	}

}

// Preinstantiate Objects //////////////////////////////////////////////////////
LEDMatrix8x8 Screen(COL_1,COL_2,COL_3,COL_4,COL_5,COL_6,COL_7,COL_8,ROW_1,ROW_2,ROW_3);