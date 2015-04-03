#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float pwm = 50;
	//Set motor direction pins to make the robot spin:
	//Left motor:
	DigitalWrite(D2, true);
	DigitalWrite(D4, false);
	//Right motor:
	DigitalWrite(D7, false);
	DigitalWrite(D8, true);
	AnalogWrite(PWM5, pwm);
	AnalogWrite(PWM6, pwm);
}

void loop()
{
}
