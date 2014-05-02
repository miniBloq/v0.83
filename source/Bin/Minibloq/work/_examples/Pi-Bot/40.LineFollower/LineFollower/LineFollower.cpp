#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		if(!(DigitalRead(D14)))
		{
			toneWithDelay(BuzzerPin, (true), 100);
		}
		else
		{
			if(!(DigitalRead(D15)))
			{
				toneWithDelay(BuzzerPin, NOTE_E5, 100);
			}
			else
			{
				if(!(DigitalRead(D16)))
				{
					toneWithDelay(BuzzerPin, NOTE_E6, 100);
				}
				else
				{
				}
			}
		}
	}
}

void loop()
{
}
