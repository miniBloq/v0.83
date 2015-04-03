#include <mbq.h>
void setup()
{
	initBoard();
	for(unsigned int _i=0; _i<(unsigned int)(3); _i++)
	{
		colorLED(WHITE);
		Delay(1);
		colorLED(BLACK);
		Delay(1);
	}
}

void loop()
{
}
