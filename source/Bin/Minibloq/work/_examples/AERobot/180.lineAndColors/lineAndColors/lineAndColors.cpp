#include <mbq.h>
void setup()
{
	initBoard();
	float line = 0;
	while(true)
	{
		line = lineSens();
		if(((int)(line)==(int)(L)))
		{
			colorLED(RED);
		}
		else
		{
			if(((int)(line)==(int)(R)))
			{
				colorLED(GREEN);
			}
			else
			{
				colorLED(BLUE);
			}
		}
	}
}

void loop()
{
}
