#include <mbq.h>


Mot8833 motor0(3, 5);
Mot8833 motor1(6, 11, false);
Servo servo0;
Servo servo1;
LiquidCrystal textLCD(12, 11, 5, 4, 3, 2);
IRrecv irReceiver(4);
HCSRSensor hcsr(0);
PingSensor ping(0);
IRRanger irRanger20to150(1, IRRanger::range20to150cm);
IRRanger irRanger10to80(1, IRRanger::range10to80cm);


void initBoard()
{
	Serial.begin(115200);

	//Uses the analog input 0, that may have the same value in some designs, but it's not so prabably:
	randomSeed(analogRead(0));

	//setPin() does not attach the servo (so the pin can be used by other library if the servo is not used).
	servo0.setPin(9);
	servo1.setPin(10);
	//;
}
