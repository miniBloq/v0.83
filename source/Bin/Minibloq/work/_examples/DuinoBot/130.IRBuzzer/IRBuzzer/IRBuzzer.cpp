#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		toneWithDelay(BuzzerPin, (irReceiver.getIRRemoteCode()*440), 100);
	}
}

void loop()
{
}
