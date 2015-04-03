#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float pwm = 0;
	while(true)
	{
		AnalogWrite(PWM10, pwm);
		pwm = (pwm+1);
		delay(100);
	}
}

void loop()
{
}
