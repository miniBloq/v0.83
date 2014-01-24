#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		motor0.setPower(pingMeasureCM(D15));
		delay(200);
	}
}

void loop()
{
}
