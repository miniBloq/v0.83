#ifndef mbqRedBot_h
#define mbqRedBot_h

#include <Arduino.h>
#include <RedBot.h>

class RedBot
{
  protected:
    float leftPower, rightPower;
    RedBotMotor motor;

  public:
	RedBotSoftwareSerial xbee;

	RedBot() : 	leftPower(0.0),
				rightPower(0.0)
	{
		xbee.begin(115200);
	}
	
	//Differential tracktion mobile robot functions:
    void move(float power);
	void move(float powerLeft, float powerRight);
    void rotate(float power);
    void brake();
    void leftBrake();
    void rightBrake();
	
	//DuinoBot compatibility functions:
	void motor0(float power);
	float motor0() const;
	void motor1(float power);
	float motor1() const;
};
#endif
