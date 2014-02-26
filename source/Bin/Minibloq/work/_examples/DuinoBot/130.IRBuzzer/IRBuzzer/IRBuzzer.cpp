#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		toneWithDelay(BuzzerPin, (irReceiver.getIRRemoteCode()*1000), 100);
	}
}

void loop()
{
}
