#ifndef mbqRedBot_h
#define mbqRedBot_h

#include <Arduino.h>
#include <RedBot.h>

#define    L_CTRL_1   2
#define    L_CTRL_2   4
#define    PWM_L      5

#define    R_CTRL_1   7
#define    R_CTRL_2   8
#define    PWM_R      6


class RedBot
{
  public:
    RedBot()
	{
	}
    void move(float power);
	void move(float powerLeft, float powerRight);
    void rotate(int power);
    void brake();
    void leftBrake();
    void rightBrake();
};
#endif
