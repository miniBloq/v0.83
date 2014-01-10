#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	float code = 0;
	while(true)
	{
		code = irReceiver.getIRRemoteCode();
		serial0.println(code);
		//toneWithDelay(BuzzerPin, (code*440), 250);
		serial0.println((50*code));
		servo0.attachAndWrite((50*code));
		delay(250);
	}
}

void loop()
{
}
