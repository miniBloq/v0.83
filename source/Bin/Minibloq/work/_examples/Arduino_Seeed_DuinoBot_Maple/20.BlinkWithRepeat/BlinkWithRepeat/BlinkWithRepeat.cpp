#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	for(unsigned int _i=0; _i<(unsigned int)(10); _i++)
	{
		DigitalWrite(D21_BiLED, true);
		delay(300);
		DigitalWrite(D21_BiLED, false);
		delay(300);
	}
}

void loop()
{
}
