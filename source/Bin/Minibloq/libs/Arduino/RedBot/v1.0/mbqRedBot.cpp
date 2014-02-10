#include <Arduino.h>
#include <RedBot.h>
#include <mbqRedBot.h>


void RedBot::move(float power)
{
}


void RedBot::move(float powerLeft, float powerRight)
{
}


void RedBot::rotate(int power)
{
}


void RedBot::brake()
{
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
	//## map power here (-100 to 100):
	motor.leftDrive((int)power);
}


float RedBot::motor0() const
{
}


void RedBot::motor1(float power)
{
}


float RedBot::motor1() const
{
}
