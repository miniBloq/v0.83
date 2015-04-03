#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		colorLED(Random(),Random(),Random());
		Delay(0.2);
	}
}

void loop()
{
}
