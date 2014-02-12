#include <RedBot.h>
#include <mbq.h>

int speed = 50;

void go()
{
	serial0.begin(115200);
	accel.enableBump();

	while(true)
	{
		robot.motor0(speed);
		robot.motor1(speed);
		if (accel.checkBump())
		{
			serial0.println("Ouch!");
			robot.motor0(0);
			robot.motor1(0);
			delay(200);
			robot.motor0(-speed);
			robot.motor1(-speed);
			delay(1000);
			robot.motor0(0);
			robot.motor1(0);
			delay(200);
			robot.motor0(speed);
			robot.motor1(-speed);
			delay(1500);
			robot.motor0(0);
			robot.motor1(0);
			delay(200);
		}
	}
}
