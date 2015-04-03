#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	for(unsigned int _i=0; _i<(unsigned int)(20); _i++)
	{
		toneWithDelay(BuzzerPin, (Random()*100), 200);
	}
}

void loop()
{
}
