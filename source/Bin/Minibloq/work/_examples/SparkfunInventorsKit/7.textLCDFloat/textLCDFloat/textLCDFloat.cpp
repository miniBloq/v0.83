#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float x = 0;
	textLCD.print("Float counter:");
	while(true)
	{
		textLCD.setCursor((int)0, (int)1);
		textLCD.print(x);
		if((x<10))
		{
			x = (x+0.1);
		}
		else
		{
			x = 0;
		}
		delay(250);
	}
}

void loop()
{
}
