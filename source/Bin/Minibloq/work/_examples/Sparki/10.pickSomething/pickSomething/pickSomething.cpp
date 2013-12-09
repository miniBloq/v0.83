#include <mbq.h>

void setup()
{
	initBoard();

	float speed = 50;
	//Open the gripper:
	robot.motorRotate(MOTOR_GRIPPER, (int)(100));
	delay(500);
	robot.motorRotate(MOTOR_GRIPPER, (int)(0));
	//Forward:
	robot.motorRotate(MOTOR_LEFT, (int)(speed));
	robot.motorRotate(MOTOR_RIGHT, (int)(-(speed)));
	delay(2000);
	//Stop:
	robot.motorRotate(MOTOR_LEFT, (int)(0));
	robot.motorRotate(MOTOR_RIGHT, (int)(0));
	//Pick something:
	robot.motorRotate(MOTOR_GRIPPER, (int)(-(100)));
	delay(400);
	robot.motorRotate(MOTOR_GRIPPER, (int)(0));
	//Turn left:
	robot.motorRotate(MOTOR_RIGHT, (int)(speed));
	delay(500);
	//Forward again:
	robot.motorRotate(MOTOR_RIGHT, (int)(-(speed)));
	delay(2000);
	//Stop everything:
	robot.motorRotate(MOTOR_LEFT, (int)(0));
	robot.motorRotate(MOTOR_RIGHT, (int)(0));
}

void loop()
{
}
