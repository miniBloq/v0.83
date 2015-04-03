#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float counter = 0;
	while(true)
	{
		serial0.println(counter);
		counter = (counter+1);
		delay(300);
	}
}

void loop()
{
}
