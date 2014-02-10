#ifndef mbqRedBot_h
#define mbqRedBot_h

#include <Arduino.h>
#include <RedBot.h>


class RedBot
{
  protected:
    RedBotMotor motor;

  public:
    RedBot()
	{
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
