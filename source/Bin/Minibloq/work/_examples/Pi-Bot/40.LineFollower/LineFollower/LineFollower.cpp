#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float fast = 30;
	float slow = 0;
	while(true)
	{
		if((DigitalRead(D14)&&!(DigitalRead(D15))&&DigitalRead(D16)))
		{
			motor0.setPower(fast);
			motor1.setPower(fast);
		}
		else
		{
			if((DigitalRead(D14)&&!(DigitalRead(D16))))
			{
				motor0.setPower(slow);
				motor1.setPower(fast);
			}
			else
			{
				if((!(DigitalRead(D14))&&DigitalRead(D16)))
				{
					motor0.setPower(fast);
					motor1.setPower(slow);
				}
				else
				{
				}
			}
		}
	}
}

void loop()
{
}
