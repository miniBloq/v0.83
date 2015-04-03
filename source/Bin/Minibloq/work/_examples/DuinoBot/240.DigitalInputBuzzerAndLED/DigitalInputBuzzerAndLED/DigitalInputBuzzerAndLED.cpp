#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		if(DigitalRead(D14))
		{
			DigitalWrite(D14, true);
			toneWithDelay(BuzzerPin, NOTE_E4, 100);
		}
		else
		{
			DigitalWrite(D14, false);
		}
	}
}

void loop()
{
}
