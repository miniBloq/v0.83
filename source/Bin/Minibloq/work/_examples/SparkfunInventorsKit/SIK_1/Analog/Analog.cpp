#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		AnalogWrite(PWM9, 255);
		delay(1000);
		DigitalWrite(D9, false);
		AnalogWrite(PWM9, 0);
		delay(1000);
	}
}

void loop()
{
}
