#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float rev_time = 2000;
	float speed = 60;
	float trigger = 50;
	DigitalWrite(D15, true);
	while(true)
	{
		if((AnalogRead(sensor0)<trigger))
		{
			motor0.setPower(speed);
		}
		else
		{
			motor0.setPower(-(speed));
			delay(rev_time);
		}
	}
}

void loop()
{
}
