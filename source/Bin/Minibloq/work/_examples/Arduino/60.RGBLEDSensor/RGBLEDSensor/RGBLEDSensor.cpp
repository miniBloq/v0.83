#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float senValue = 0;
	while(true)
	{
		senValue = AnalogRead(sensor0);
		if((senValue<33))
		{
			AnalogWrite(PWM9, senValue);
			AnalogWrite(PWM10, 0);
			AnalogWrite(PWM11, 0);
		}
		else
		{
			if((senValue<66))
			{
				AnalogWrite(PWM9, 0);
				AnalogWrite(PWM10, senValue);
				AnalogWrite(PWM11, 0);
			}
			else
			{
				AnalogWrite(PWM9, 0);
				AnalogWrite(PWM10, 0);
				AnalogWrite(PWM11, senValue);
			}
		}
		delay(300);
	}
}

void loop()
{
}
