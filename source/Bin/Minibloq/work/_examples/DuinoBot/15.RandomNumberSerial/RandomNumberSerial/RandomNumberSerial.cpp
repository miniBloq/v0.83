#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		serial0.println(Random());
		delay(300);
	}
}

void loop()
{
}
