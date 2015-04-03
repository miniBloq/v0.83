#include <mbq.h>


Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;
RedBot robot;
IRrecv irReceiver(11);
PingSensor ping(0);
IRRanger irRanger20to150(1, IRRanger::range20to150cm);
IRRanger irRanger10to80(1, IRRanger::range10to80cm);
RedBotAccel accel;


void initBoard()
{
	Serial.begin(115200);

	//Uses the analog input 0, that may have the same value in some designs, but it's not so prabably:
	randomSeed(analogRead(0));

	//setPin() does not attach the servo (so the pin can be used by other library if the servo is not used).
	servo0.setPin(3);
	servo1.setPin(9);
	servo2.setPin(10);
	servo3.setPin(11);
	//;
}
