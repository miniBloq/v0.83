#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		DigitalWrite(D13_LED, DigitalRead(D2));
	}
}

void loop()
{
}
