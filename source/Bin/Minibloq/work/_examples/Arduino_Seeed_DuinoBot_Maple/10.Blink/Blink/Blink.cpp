#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
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
