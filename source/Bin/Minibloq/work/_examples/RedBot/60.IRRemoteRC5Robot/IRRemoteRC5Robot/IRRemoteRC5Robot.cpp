#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float vel = 80;
	float spinVel = 70;
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
		if(((int)(code)==(int)(4)))
		{
			robot.motor0(-(spinVel));
			robot.motor1(spinVel);
		}
		else
		{
		}
		if(((int)(code)==(int)(6)))
		{
			robot.motor0(spinVel);
			robot.motor1(-(spinVel));
		}
		else
		{
		}
		if(((int)(code)==(int)(3)))
		{
			robot.motor0(vel);
			robot.motor1((vel*0.7));
		}
		else
		{
		}
		if(((int)(code)==(int)(1)))
		{
			robot.motor0((vel*0.7));
			robot.motor1(vel);
		}
		else
		{
		}
		if(((int)(code)==(int)(5)))
		{
			toneWithDelay(BuzzerPin, 880, 150);
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
