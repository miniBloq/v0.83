#include <mbq.h>
void setup()
{
	initBoard();
	while(true)
	{
		colorLED(RED);
		Delay((Random()/100));
		colorLED(BLUE);
		Delay((Random()/100));
	}
}

void loop()
{
}
