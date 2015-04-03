#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float pwm = 0;
	while(true)
	{
		if((pwm<100))
		{
			pwm = (pwm+5);
		}
		else
		{
			pwm = 0;
		}
		AnalogWrite(PWM10, pwm);
		delay(100);
	}
}

void loop()
{
}
