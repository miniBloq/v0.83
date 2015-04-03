#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float factor = 0.1;
	while(true)
	{
		toneWithDelay(BuzzerPin, (abs(accel.readZ())*factor), 50);
	}
}

void loop()
{
}
