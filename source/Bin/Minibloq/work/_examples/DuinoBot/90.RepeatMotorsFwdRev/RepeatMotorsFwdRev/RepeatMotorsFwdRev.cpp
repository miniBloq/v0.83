#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	for(unsigned int _i=0; _i<(unsigned int)(3); _i++)
	{
		motor0.setPower(50);
		motor1.setPower(50);
		delay(1000);
		motor0.setPower(0);
		motor1.setPower(0);
		delay(200);
		motor0.setPower(-(50));
		motor1.setPower(-(50));
		delay(1000);
	}
	motor0.setPower(0);
	motor1.setPower(0);
}

void loop()
{
}
