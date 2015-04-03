#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		AnalogWrite(PWM9, Random());
		AnalogWrite(PWM10, Random());
		AnalogWrite(PWM11, Random());
		delay(300);
	}
}

void loop()
{
}
