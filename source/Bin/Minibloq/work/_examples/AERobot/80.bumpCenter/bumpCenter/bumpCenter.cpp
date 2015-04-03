#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		if(bumpSens(CENTER))
		{
			colorLED(RED);
			Delay(0.3);
		}
		else
		{
			colorLED(BLACK);
		}
	}
}

void loop()
{
}
