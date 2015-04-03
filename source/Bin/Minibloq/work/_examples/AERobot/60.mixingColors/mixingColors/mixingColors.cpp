#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		colorLED(20,30,14);
		Delay(0.5);
		colorLED(50,0,65);
		Delay(0.5);
	}
}

void loop()
{
}
