#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float irCode = 0;
	float ultrasonic = 0;
	servo0.attachAndWrite(60);
	motor0.setPower(50);
	motor1.setPower(50);
	while(true)
	{
		irCode = irReceiver.getIRRemoteCode();
		toneWithDelay(BuzzerPin, (440*irCode), 200);
		servo0.attachAndWrite((irCode*10));
		if(((int)(irCode)==(int)(5)))
		{
			toneWithDelay(BuzzerPin, 440, 100);
			ultrasonic = pingMeasureCM(D15);
			serial0.println(ultrasonic);
			motor0.setPower(ultrasonic);
			motor1.setPower(ultrasonic);
		}
		else
		{
		}
	}
}

void loop()
{
}
