#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		toneWithDelay(BuzzerPin, (hcsrMeasureCM(D7)*100), 100);
	}
}

void loop()
{
}
