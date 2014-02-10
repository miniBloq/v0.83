#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while((robot.motor0()<100))
	{
		serial0.println(robot.motor0());
		robot.motor0((robot.motor0()+1));
		delay(1000);
	}
	robot.motor0(0);
}

void loop()
{
}
