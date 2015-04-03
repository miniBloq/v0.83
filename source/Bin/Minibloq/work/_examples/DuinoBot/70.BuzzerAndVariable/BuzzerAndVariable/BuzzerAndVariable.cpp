#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float note = 0;
	while((note<25))
	{
		toneWithDelay(BuzzerPin, (note*50), 200);
		note = (note+1);
	}
}

void loop()
{
}
