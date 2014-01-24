#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float pwm = 0;
	while(true)
	{
		pwm = (pwm+1);
		AnalogWrite(PWM9, pwm);
		delay(100);
	}
}

void loop()
{
}
