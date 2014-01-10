#include <mbq.h>


SparkiClass robot;


void initBoard()
{
	robot.begin();
	robot.setMotorInvertedSpeedSign(true);
	delay(1000);
}
