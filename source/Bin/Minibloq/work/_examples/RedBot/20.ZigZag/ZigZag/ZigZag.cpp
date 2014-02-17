#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float speed = 50;
	float rotationTime = 250;
	float forwardTime = 1100;
	for(unsigned int _i=0; _i<(unsigned int)(50); _i++)
	{
		robot.motor0(-(speed));
		robot.motor1(speed);
		delay(rotationTime);
		robot.motor0(speed);
		delay(forwardTime);
		robot.motor1(-(speed));
		delay(rotationTime);
		robot.motor1(speed);
		delay(forwardTime);
	}
}

void loop()
{
}
