#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		while(DigitalRead(D14))
		{
			toneWithDelay(BuzzerPin, 440, 20);
		}
		//serial0.println(pingMeasureCM(D15));
		//delay(20);
	}
}

void loop()
{
}
