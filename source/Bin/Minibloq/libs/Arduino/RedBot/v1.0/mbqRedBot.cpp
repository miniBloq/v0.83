#include <Arduino.h>
#include <RedBot.h>
#include <mbqRedBot.h>

const int maxPWMValue = 255;

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
	leftPower = 0.0;
	rightPower = 0.0;
}


void RedBot::leftBrake()
{
	motor.leftBrake();
	leftPower = 0.0;
}


void RedBot::rightBrake()
{
	motor.rightBrake();
	rightPower = 0.0;
}


void RedBot::motor0(float power)
{
	int intPower = map((int)power, -100, 100, -255, 255);
	motor.leftDrive((int)intPower);
	leftPower = power;
}


float RedBot::motor0() const
{
	return leftPower;
}


void RedBot::motor1(float power)
{
	int intPower = map((int)power, -100, 100, -255, 255);
	motor.rightDrive((int)intPower);
	rightPower = power;
}


float RedBot::motor1() const
{
	return rightPower;
}
