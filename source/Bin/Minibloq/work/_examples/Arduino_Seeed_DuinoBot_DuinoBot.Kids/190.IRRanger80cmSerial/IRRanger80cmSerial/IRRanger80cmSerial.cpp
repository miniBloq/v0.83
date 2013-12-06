#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		serial0.println(irRanger10to80.measureCM(sensor1));
		delay(200);
	}
}

void loop()
{
}
