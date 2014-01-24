#include <mbq.h>

void setup()
{
	initBoard();
	float distance = 15;
	float speed = 100;
	while(true)
	{
		robot.motorRotate(MOTOR_LEFT, (int)(speed));
		robot.motorRotate(MOTOR_RIGHT, (int)(speed));
		if((robot.ping_single()<distance))
		{
			robot.motorRotate(MOTOR_LEFT, (int)(0));
			robot.motorRotate(MOTOR_RIGHT, (int)(-(speed)));
			while((robot.ping_single()<distance))
			{
				delay(200);
			}
		}
		else
		{
		}
	}
}

void loop()
{
}
