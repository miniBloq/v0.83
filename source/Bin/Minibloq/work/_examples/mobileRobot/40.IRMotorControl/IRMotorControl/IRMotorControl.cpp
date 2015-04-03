#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float code = 0;
	while(true)
	{
		code = irReceiver.getIRRemoteCode();
		if(((int)(code)==(int)(10)))
		{
			robot.motor0(0);
		}
		else
		{
			if((code>0))
			{
				robot.motor0((max(code, 1)*10));
			}
			else
			{
			}
		}
		delay(100);
	}
}

void loop()
{
}
