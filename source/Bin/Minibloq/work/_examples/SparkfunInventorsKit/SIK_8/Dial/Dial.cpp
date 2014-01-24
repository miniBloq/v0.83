#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float sensorValue = 0;
	while(true)
	{
		sensorValue = AnalogRead(sensor0);
		DigitalWrite(D9, true);
		delay((sensorValue*10));
		DigitalWrite(D9, false);
		delay((sensorValue*10));
	}
}

void loop()
{
}
