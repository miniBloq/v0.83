#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	while(true)
	{
		serial0.println("X, Y, Z accel values:");
		serial0.println(accel.readX());
		serial0.println(accel.readY());
		serial0.println(accel.readZ());
		//Blank line here:
		serial0.println("");
		delay(600);
	}
}

void loop()
{
}
