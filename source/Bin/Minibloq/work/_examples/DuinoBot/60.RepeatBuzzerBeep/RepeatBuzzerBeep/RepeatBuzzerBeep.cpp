#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	for(unsigned int _i=0; _i<(unsigned int)(5); _i++)
	{
		toneWithDelay(BuzzerPin, NOTE_C4, 300);
		toneWithDelay(BuzzerPin, NOTE_B4, 200);
	}
}

void loop()
{
}
