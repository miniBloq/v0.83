#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		serial0.println(pingMeasureCM(D15));
		delay(200);
	}
}

void loop()
{
}
