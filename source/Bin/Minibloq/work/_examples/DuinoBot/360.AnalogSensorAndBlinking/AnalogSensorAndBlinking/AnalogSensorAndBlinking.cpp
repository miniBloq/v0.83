#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float s0 = 0;
	while(true)
	{
		s0 = AnalogRead(sensor0);
		DigitalWrite(D13_LED, true);
		delay((s0*5));
		DigitalWrite(D13_LED, false);
		delay((s0*5));
	}
}

void loop()
{
}
