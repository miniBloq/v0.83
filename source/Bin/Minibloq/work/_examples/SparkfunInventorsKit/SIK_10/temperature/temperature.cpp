#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float temperature = 0;
	while(true)
	{
		temperature = (((AnalogRead(sensor0)*0.049951187)-0.5)*100);
		serial0.println(temperature);
		delay(1000);
	}
}

void loop()
{
}
