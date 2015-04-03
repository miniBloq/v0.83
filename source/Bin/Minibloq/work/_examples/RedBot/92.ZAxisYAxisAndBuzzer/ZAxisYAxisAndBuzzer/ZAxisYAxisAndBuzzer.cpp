#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		toneWithDelay(BuzzerPin, (abs(accel.readZ())*0.1), (abs(accel.readY())*0.01));
	}
}

void loop()
{
}
