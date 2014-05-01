#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		serial0.println(AnalogRead(sensor3));
		delay(100);
	}
}

void loop()
{
}
