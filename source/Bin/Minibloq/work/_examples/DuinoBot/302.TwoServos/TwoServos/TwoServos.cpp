#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	servo0.attachAndWrite(0);
	servo1.attachAndWrite(0);
	for(unsigned int _i=0; _i<(unsigned int)(180); _i++)
	{
		servo0.attachAndWrite((servo0.read()+1));
		servo1.attachAndWrite((servo1.read()+1));
		delay(15);
	}
	for(unsigned int _i=0; _i<(unsigned int)(180); _i++)
	{
		servo0.attachAndWrite((servo0.read()-1));
		servo1.attachAndWrite((servo1.read()-1));
		delay(15);
	}
}

void loop()
{
}
