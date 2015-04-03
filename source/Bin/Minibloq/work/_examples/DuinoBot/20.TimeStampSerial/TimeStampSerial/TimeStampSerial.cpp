#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		serial0.println(timeStamp());
		delay(300);
	}
}

void loop()
{
}
