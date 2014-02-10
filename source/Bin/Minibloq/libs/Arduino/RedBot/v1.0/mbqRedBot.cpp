#include <Arduino.h>
#include <RedBot.h>
#include <mbqRedBot.h>


void RedBot::move(float power)
{
	motor0(power);
	motor1(power);
}


void RedBot::move(float powerLeft, float powerRight)
{
	motor0(powerLeft);
	motor1(powerRight);
}


void RedBot::rotate(float power)
{
	if (power < 0)
	{
		//Positive angle (counterclockwise):
		motor0(-power); //Left
		motor1(power); //Right
	}
	else
	{
		//Negative angle (clockwise):
		motor0(power); //Left
		motor1(-power); //Right
	}
}


void RedBot::brake()
{
	motor.brake();
}


void RedBot::leftBrake()
{
	motor.leftBrake();
}


void RedBot::rightBrake()
{
	motor.rightBrake();
}


void RedBot::motor0(float power)
{
	int intPower = map((int)power, -100, 100, -255, 255);
	motor.leftDrive((int)intPower);
}


float RedBot::motor0() const
{
	//float power = map(motor.getLeftSpeed(), -255, 255, -100, 100);
	float power = (float)motor.getLeftSpeed();
	return power;
}


void RedBot::motor1(float power)
{
	int intPower = map((int)power, -100, 100, -255, 255);
	motor.rightDrive((int)intPower);
}


float RedBot::motor1() const
{
	float power = map(motor.getRightSpeed(), -255, 255, -100, 100);
	return power;
}
