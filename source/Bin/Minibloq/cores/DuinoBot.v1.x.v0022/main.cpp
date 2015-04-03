#include <WProgram.h>

int main(void)
{

	init();
	
	//##RG.Labs.20110318 begin
	// This must be included, otherwise the optimizer make strange things.
	Serial.begin(9600);
	//##RG.Labs.20110318 end
	
	setup();

	for (;;)
		loop();

	return 0;
}

