#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		toneWithDelay(BuzzerPin, (pingMeasureCM(D15)*400), 250);
	}
}

void loop()
{
}
