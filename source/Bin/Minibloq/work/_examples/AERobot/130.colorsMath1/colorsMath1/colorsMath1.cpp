#include <mbq.h>
void setup()
{
	initBoard();
	float green = 0;
	while(true)
	{
		colorLED(0,green,0);
		green = (green+10);
		if((green>100))
		{
			green = 0;
		}
		else
		{
		}
		Delay(0.5);
	}
}

void loop()
{
}
