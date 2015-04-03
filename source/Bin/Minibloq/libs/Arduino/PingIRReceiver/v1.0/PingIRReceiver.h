#ifndef PingIRReceiver_h
#define PingIRReceiver_h

#include <mbq.h>

//This function does not belong to the Ping sensor, nor to the IRReceiver: It's
//a product of the relation between them and some boards. That's why it's a new
//library itself. It's a function to prevent an incompatibility between
//the IRReceiver and the Ping classes: 
long pingMeasureCM(int pin)
{
	if (irReceiver.isEnabledIRIn())
	{
		irReceiver.disableIRIn();
		delay(10);
	}
	return ping.measureCM(pin);
}

long hcsrMeasureCM(int pin)
{
	if (irReceiver.isEnabledIRIn())
	{
		irReceiver.disableIRIn();
		delay(10);
	}
	return hcsr.measureCM(pin);
}

#endif
