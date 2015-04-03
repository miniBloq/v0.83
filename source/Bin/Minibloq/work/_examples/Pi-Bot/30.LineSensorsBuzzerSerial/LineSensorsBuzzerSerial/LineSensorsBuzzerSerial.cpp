#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		if(!(DigitalRead(D14)))
		{
			toneWithDelay(BuzzerPin, 440, 100);
		}
		else
		{
			if(!(DigitalRead(D15)))
			{
				toneWithDelay(BuzzerPin, 880, 100);
			}
			else
			{
				if(!(DigitalRead(D16)))
				{
					toneWithDelay(BuzzerPin, 1760, 100);
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
