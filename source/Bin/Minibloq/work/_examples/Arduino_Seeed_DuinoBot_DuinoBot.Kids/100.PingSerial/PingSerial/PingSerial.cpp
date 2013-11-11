#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		motor0.setPower(0);
		toneWithDelay(BuzzerPin, 0, 0);
		servo1.attachAndWrite(0);
		serial0.println(pingMeasureCM(D15));
		delay(irReceiver.getIRRemoteCode());
	}
}

void loop()
{
}
