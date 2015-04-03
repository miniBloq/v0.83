#include <mbq.h>
#include <PingIRReceiver.h>

void setup()
{
	initBoard();
	float speed = 40;
	while(true)
	{
		motor0.setPower(speed);
		motor1.setPower(speed);
		motor2.setPower(speed);
		motor3.setPower(speed);
		delay(4000);
		motor0.setPower(0);
		motor1.setPower(0);
		motor2.setPower(0);
		motor3.setPower(0);
		delay(200);
		motor0.setPower(-(speed));
		motor1.setPower(-(speed));
		motor2.setPower(-(speed));
		motor3.setPower(-(speed));
		delay(4000);
		motor0.setPower(0);
		motor1.setPower(0);
		motor2.setPower(0);
		motor3.setPower(0);
	}
}

void loop()
{
}
