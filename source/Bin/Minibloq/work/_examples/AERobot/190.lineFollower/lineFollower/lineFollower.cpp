#include <mbq.h>
void setup()
{
	initBoard();
	float line = 0;
	while(true)
	{
		line = lineSens();
		if(((int)(line)==(int)(L)))
		{
			move(TURN_LEFT);
		}
		else
		{
			if(((int)(line)==(int)(R)))
			{
				move(TURN_RIGHT);
			}
			else
			{
				move(FORWARD);
			}
		}
	}
}

void loop()
{
}
