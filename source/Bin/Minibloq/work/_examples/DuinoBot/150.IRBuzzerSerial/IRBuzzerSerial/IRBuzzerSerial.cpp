#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float code = 0;
	while(true)
	{
		code = irReceiver.getIRRemoteCode();
		toneWithDelay(BuzzerPin, (code*440), 250);
		serial0.println(code);
	}
}

void loop()
{
}
