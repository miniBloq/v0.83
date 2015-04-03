#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float x = 0;
	float y = 0;
	while(true)
	{
		y = abs((sin(x)*100));
		AnalogWrite(PWM10, y);
		x = (x+0.01);
		delay(10);
	}
}

void loop()
{
}
