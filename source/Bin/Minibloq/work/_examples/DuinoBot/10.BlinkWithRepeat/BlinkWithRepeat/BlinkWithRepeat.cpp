#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	for(unsigned int _i=0; _i<(unsigned int)(10); _i++)
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
