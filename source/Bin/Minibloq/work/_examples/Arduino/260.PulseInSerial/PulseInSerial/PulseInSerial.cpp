#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	while(true)
	{
		serial0.println(PulseIn(D15, true, 200000));
	}
}

void loop()
{
}
