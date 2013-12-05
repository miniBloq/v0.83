#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();

	servo0.attachAndWrite(0);
	for(unsigned int _i=0; _i<(unsigned int)(90); _i++)
	{
		servo0.attachAndWrite((servo0.read()+2));
		delay(200);
	}
}

void loop()
{
}
