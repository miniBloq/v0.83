#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		if((distSens(CENTER)<400))
		{
			colorLED(BLUE);
			move(FORWARD);
		}
		else
		{
			while(((distSens(CENTER)>500)||bumpSens(RIGHT)))
			{
				colorLED(RED);
				move(TURN_LEFT);
			}
		}
	}
}

void loop()
{
}
