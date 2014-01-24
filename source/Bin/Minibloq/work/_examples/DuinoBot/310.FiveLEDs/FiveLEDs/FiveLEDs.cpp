#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float ms = 300;
	while(true)
	{
		DigitalWrite(D12, false);
		DigitalWrite(D8, true);
		delay(ms);
		DigitalWrite(D8, false);
		DigitalWrite(D9, true);
		delay(ms);
		DigitalWrite(D9, false);
		DigitalWrite(D10, true);
		delay(ms);
		DigitalWrite(D10, false);
		DigitalWrite(D11, true);
		delay(ms);
		DigitalWrite(D11, false);
		DigitalWrite(D12, true);
		delay(ms);
	}
}

void loop()
{
}
