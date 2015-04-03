#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float speed = 70;
	float distance = 0;
	float minDist = 18;
	while(true)
	{
		distance = hcsrMeasureCM(D7);
		if((distance>minDist))
		{
			motor0.setPower(speed);
			motor1.setPower(speed);
		}
		else
		{
			motor0.setPower(0);
			motor1.setPower(0);
			delay(100);
			motor0.setPower(-(100));
			motor1.setPower(-(100));
			delay(800);
			motor0.setPower(speed);
			motor1.setPower(0);
			delay(450);
		}
		delay(100);
	}
}

void loop()
{
}
