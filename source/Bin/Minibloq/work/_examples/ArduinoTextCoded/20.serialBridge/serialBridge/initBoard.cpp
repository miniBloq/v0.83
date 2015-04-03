#include <mbq.h>


DCMotor motor0(27, 23, 24);
DCMotor motor1(28, 25, 26);
Servo servo0;
Servo servo1;
IRrecv irReceiver(D14);
PingSensor ping(0);
IRRanger irRanger20to150(1, IRRanger::range20to150cm);
IRRanger irRanger10to80(1, IRRanger::range10to80cm);


void initBoard()
{
	Serial.begin(115200);
	Serial1.begin(115200);

	randomSeed(analogRead(0));

	//This is for the Multiplo robots:
	//motor1.setClockwise(false);
	//setPin() does not attach the servo (so the pin can be used by other library if the servo is not used).
	servo0.setPin(16);
	servo1.setPin(17);
}
