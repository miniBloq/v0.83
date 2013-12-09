#include <mbq.h>

void setup()
{
	initBoard();

	float speed = 80;
	//Open the gripper:
	robot.motorRotate(MOTOR_GRIPPER, (int)(-(100)));
	delay(800);
	robot.motorRotate(MOTOR_GRIPPER, (int)(0));
	//Forward:
	robot.motorRotate(MOTOR_LEFT, (int)(-(speed)));
	robot.motorRotate(MOTOR_RIGHT, (int)(speed));
	delay(3000);
	//Stop:
	robot.motorRotate(MOTOR_LEFT, (int)(0));
	robot.motorRotate(MOTOR_RIGHT, (int)(0));
	//Pick something:
	//while(true)
	//{
	//}
	robot.motorRotate(MOTOR_GRIPPER, (int)(1000));
	delay(800);
	robot.motorRotate(MOTOR_GRIPPER, (int)(0));
	//Turn left:
	robot.motorRotate(MOTOR_LEFT, (int)(-(speed)));
	delay(4000);
	//Forward again:
	robot.motorRotate(MOTOR_RIGHT, (int)(speed));
	delay(3000);
	//Stop everything:
	robot.motorRotate(MOTOR_LEFT, (int)(0));
	robot.motorRotate(MOTOR_RIGHT, (int)(0));
}

void loop()
{
}
