#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		toneWithDelay(BuzzerPin, (pingMeasureCM(D15)*400), 100);
		//serial0.println(pingMeasureCM(D15));
		//delay(200);
	}
}

void loop()
{
}
