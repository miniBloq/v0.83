#include <WProgram.h>
#include <stdlib.h>
#include <minibloq.h>
#include <pitches.h>
#include <DCMotor.h>
#include <Servo.h>
#include <IRremote.h>
#include <Ping.h>
#include <IRRanger.h>




void setup()
{
	initBoard();

	while(true)
	{
		serial0.println(Random());
		delay(300);
	}
}

void loop()
{
}
