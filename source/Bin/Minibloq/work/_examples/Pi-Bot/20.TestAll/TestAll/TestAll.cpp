#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float ultrasonic = 0;
	motor0.setPower(80);
	motor1.setPower(80);
	delay(1500);
	motor0.setPower(-(50));
	motor1.setPower(-(50));
	delay(1500);
	motor0.setPower(0);
	motor1.setPower(0);
	while(true)
	{
		ultrasonic = hcsrMeasureCM(D7);
		serial0.println(ultrasonic);
		if((ultrasonic<15))
		{
			servo0.attachAndWrite(0);
			servo1.attachAndWrite(50);
			toneWithDelay(BuzzerPin, 880, 50);
			DigitalWrite(D13_LED, true);
		}
		else
		{
			servo0.attachAndWrite(50);
			servo1.attachAndWrite(0);
			DigitalWrite(D13_LED, false);
		}
		delay(100);
	}
}

void loop()
{
}
