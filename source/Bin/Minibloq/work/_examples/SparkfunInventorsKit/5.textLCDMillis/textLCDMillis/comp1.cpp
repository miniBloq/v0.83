#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	textLCD.print("Milliseconds from");
	while(true)
	{
		textLCD.setCursor((int)0, (int)1.0);
		textLCD.print("reset:");
		textLCD.setCursor((int)6, (int)1);
		textLCD.print(timeStamp());
		delay(250);
	}
}

void loop()
{
}
