#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float vel = 50;
	float code = 0;
	while(true)
	{
		code = irReceiver.getIRRemoteCode();
		if(((int)(code)==(int)(2)))
		{
			motor0.setPower(vel);
			motor1.setPower(vel);
		}
		else
		{
		}
		if(((int)(code)==(int)(8)))
		{
			motor0.setPower(-(vel));
			motor1.setPower(-(vel));
		}
		else
		{
		}
		if(((int)(code)==(int)(6)))
		{
			motor0.setPower(-(vel));
			motor1.setPower(vel);
		}
		else
		{
		}
		if(((int)(code)==(int)(4)))
		{
			motor0.setPower(vel);
			motor1.setPower(-(vel));
		}
		else
		{
		}
		if(((int)(code)==(int)(5)))
		{
			motor0.setPower(0);
			motor1.setPower(0);
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
