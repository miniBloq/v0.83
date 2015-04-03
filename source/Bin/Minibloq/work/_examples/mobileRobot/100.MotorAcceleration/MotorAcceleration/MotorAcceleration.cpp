#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	motor0.setPower(-(100));
	motor1.setPower(motor0.getPower());
	while((motor0.getPower()<100))
	{
		motor0.setPower((motor0.getPower()+1));
		motor1.setPower(motor0.getPower());
		serial0.println(motor0.getPower());
		delay(100);
	}
	motor0.setPower(0);
	motor1.setPower(0);
}

void loop()
{
}
