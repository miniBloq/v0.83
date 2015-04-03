#include <mbq.h>
void setup()
{
	initBoard();
	float time = 0.01;
	while(true)
	{
		if(bumpSens(LEFT))
		{
			colorLED(RED);
			Delay(time);
		}
		else
		{
		}
		if(bumpSens(CENTER))
		{
			colorLED(BLUE);
			Delay(time);
		}
		else
		{
		}
		if(bumpSens(RIGHT))
		{
			colorLED(GREEN);
			Delay(time);
		}
		else
		{
		}
		colorLED(BLACK);
	}
}

void loop()
{
}
