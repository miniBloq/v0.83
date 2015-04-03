#include <mbq.h>


AF_DCMotor afMotor0(1, MOTOR12_1KHZ);
AF_DCMotor afMotor1(2, MOTOR12_1KHZ);
AF_DCMotor afMotor2(3);
AF_DCMotor afMotor3(4);
DCMotorWrap motor0(&afMotor0);
DCMotorWrap motor1(&afMotor1);
DCMotorWrap motor2(&afMotor2);
DCMotorWrap motor3(&afMotor3);
Servo servo0;
Servo servo1;
IRrecv irReceiver(14);
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
}
