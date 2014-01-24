#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		DigitalWrite(D13_LED, true);
		delay(300);
		DigitalWrite(D13_LED, false);
		delay(300);
	}
}

void loop()
{
}
