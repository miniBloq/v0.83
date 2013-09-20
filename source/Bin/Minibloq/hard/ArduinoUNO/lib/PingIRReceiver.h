#ifndef PingIRReceiver_h
#define PingIRReceiver_h

#include <mbq.h>

//This is the correct way to add functions which depends on instances from other libraries for this specific board: It
//has to see if those libraries were already included:
#ifdef IRremote_h
#ifdef Ping_h

//This function does not belong to the Ping sensor, nor to the IRReceiver: It's
//a product of the relation between them and some boards. That's why it's here,
//in the board libraries. It's a function to prevent an incompatibility between
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

#endif
#endif
#endif
