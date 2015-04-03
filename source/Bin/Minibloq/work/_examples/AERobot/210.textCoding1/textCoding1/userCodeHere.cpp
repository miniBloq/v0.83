#include <mbq.h>

//User function:
void blink3times()
{
	for (int i=0; i<3; i++)
	{
		colorLED(RED);
		Delay(0.3);
		colorLED(BLACK);
		Delay(0.3);
	}
}

void go()
{
	//Very important: don't forget this line!
	initBoard();

	//Just turn on the LED with a nice color:
	colorLED(128, 0, 64);
	
	//Main application cycle:
	while(true)
	{
		move(FORWARD);
		while (!bumpSens(CENTER));

		move(STOP);
		blink3times();
	}
}