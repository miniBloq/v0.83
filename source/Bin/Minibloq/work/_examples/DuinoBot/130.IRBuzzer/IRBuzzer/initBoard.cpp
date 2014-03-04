#include <mbq.h>


RedBot robot;
IRrecv irReceiver(11);
PingSensor ping(0);
IRRanger irRanger20to150(1, IRRanger::range20to150cm);
IRRanger irRanger10to80(1, IRRanger::range10to80cm);


void initBoard()
{
	Serial.begin(115200);

	//Uses the analog input 0, that may have the same value in some designs, but it's not so prabably:
	randomSeed(analogRead(0));

	//;
}
