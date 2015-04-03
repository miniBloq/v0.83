#include <mbq.h>
void setup()
{
	initBoard();
	move(FORWARD);
	Delay(3);
	move(TURN_RIGHT);
	Delay(2);
	move(BACKWARD);
	Delay(2.5);
	move(TURN_LEFT);
	Delay(2);
	move(STOP);
}

void loop()
{
}
