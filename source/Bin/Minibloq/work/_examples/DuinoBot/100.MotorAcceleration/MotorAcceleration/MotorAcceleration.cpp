#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	robot.motor0(-(100));
	while((robot.motor0()<100))
	{
		robot.motor0((robot.motor0()+1));
		serial0.println(robot.motor0());
		delay(100);
	}
	robot.motor0(0);
}

void loop()
{
}
