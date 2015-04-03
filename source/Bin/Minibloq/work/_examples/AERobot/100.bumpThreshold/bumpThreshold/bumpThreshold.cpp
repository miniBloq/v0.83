#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		if((distSens(CENTER)>400))
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
