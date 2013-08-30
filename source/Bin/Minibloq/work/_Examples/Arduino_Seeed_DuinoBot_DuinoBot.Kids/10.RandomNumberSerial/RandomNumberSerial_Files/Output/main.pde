#include "stdlib.h"
#include "IRremote.h"
#include "pitches.h"
#include "Minibloq.h"


void setup()
{
	initBoard();

	motor0.setSpeed(0);
	while(true)
	{
		serial0.println(Random());
		delay(300);
	}
}

void loop()
{
}
