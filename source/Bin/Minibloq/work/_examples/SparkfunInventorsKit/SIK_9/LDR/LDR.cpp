#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		AnalogWrite(PWM9, AnalogRead(sensor0));
	}
}

void loop()
{
}
