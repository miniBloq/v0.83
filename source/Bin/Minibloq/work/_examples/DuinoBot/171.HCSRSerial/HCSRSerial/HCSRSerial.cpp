#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		serial0.println(hcsrMeasureCM(D7));
		delay(200);
	}
}

void loop()
{
}
