#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float thr = 10000;
	while(true)
	{
		if((accel.readX()>thr))
		{
			serial0.println("Gravity force in X direction!");
			delay(100);
		}
		else
		{
		}
	}
}

void loop()
{
}
