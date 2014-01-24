#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		serial0.println("These are serial emoticons:");
		serial0.println(":)");
		delay(300);
		serial0.println(":|");
		delay(300);
		serial0.println(":(");
		delay(300);
	}
}

void loop()
{
}
