#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		serial0.println(irRanger20to150.measureCM(sensor0));
		delay(200);
	}
}

void loop()
{
}
