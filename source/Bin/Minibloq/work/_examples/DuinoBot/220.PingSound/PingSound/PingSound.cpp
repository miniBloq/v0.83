#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		toneWithDelay(BuzzerPin, (pingMeasureCM(D15)*400), 50);
	}
}

void loop()
{
}
