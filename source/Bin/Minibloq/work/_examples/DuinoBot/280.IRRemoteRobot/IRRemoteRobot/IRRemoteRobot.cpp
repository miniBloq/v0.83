#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float vel = 45;
	float code = 0;
	while(true)
	{
		code = irReceiver.getIRRemoteCode();
		if(((int)(code)==(int)(2)))
		{
			robot.motor0(vel);
			robot.motor1(vel);
		}
		else
		{
		}
		if(((int)(code)==(int)(8)))
		{
			robot.motor0(-(vel));
			robot.motor1(-(vel));
		}
		else
		{
		}
		if(((int)(code)==(int)(6)))
		{
			robot.motor0(-(vel));
			robot.motor1(vel);
		}
		else
		{
		}
		if(((int)(code)==(int)(4)))
		{
			robot.motor0(vel);
			robot.motor1(-(vel));
		}
		else
		{
		}
		if(((int)(code)==(int)(5)))
		{
			robot.motor0(0);
			robot.motor1(0);
		}
		else
		{
		}
		delay(200);
	}
}

void loop()
{
}
