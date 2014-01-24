#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while((motor0.getPower()<100))
	{
		motor0.setPower((motor0.getPower()+1));
		delay(100);
	}
	motor0.setPower(0);
}

void loop()
{
}
