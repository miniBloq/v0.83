#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	motor0.setPower(45);
	delay(1000);
	motor0.setPower(-(45));
	delay(1000);
	motor0.setPower(0);
}

void loop()
{
}
