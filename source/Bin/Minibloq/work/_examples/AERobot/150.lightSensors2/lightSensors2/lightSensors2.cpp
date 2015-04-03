#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		if((lightSens(LEFT)>lightSens(RIGHT)))
		{
			move(TURN_RIGHT);
		}
		else
		{
			move(TURN_LEFT);
		}
	}
}

void loop()
{
}
