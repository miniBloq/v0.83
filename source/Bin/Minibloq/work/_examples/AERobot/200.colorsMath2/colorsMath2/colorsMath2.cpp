#include <mbq.h>
void setup()
{
	initBoard();
	float x = 0;
	while(true)
	{
		colorLED(abs((sin(x)*25)),abs((cos(x)*25)),0);
		x = (x+0.1);
		Delay(0.2);
	}
}

void loop()
{
}
