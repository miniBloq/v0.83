#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	servo3.attachAndWrite(0);
	for(unsigned int _i=0; _i<(unsigned int)(180); _i++)
	{
		servo3.attachAndWrite((servo3.read()+1));
		delay(15);
	}
	for(unsigned int _i=0; _i<(unsigned int)(180); _i++)
	{
		servo3.attachAndWrite((servo3.read()-1));
		delay(15);
	}
}

void loop()
{
}
