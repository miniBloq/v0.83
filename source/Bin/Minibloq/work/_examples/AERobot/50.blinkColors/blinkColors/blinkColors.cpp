#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		colorLED(RED);
		Delay(1);
		colorLED(GREEN);
		Delay(1);
		colorLED(BLUE);
		Delay(1);
	}
}

void loop()
{
}
