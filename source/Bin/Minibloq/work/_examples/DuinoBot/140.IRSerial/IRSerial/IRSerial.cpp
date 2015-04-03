#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		serial0.println(irReceiver.getIRRemoteCode());
		delay(200);
	}
}

void loop()
{
}
