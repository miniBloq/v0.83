#include <mbq.h>

void go()
{
	serial0.begin(115200);
	serial1.begin(115200);

	while(true)
	{
		if (serial0.available()) 
		{
			int inByte = serial0.read();
			serial1.print((char)inByte); 
		}
		if (serial1.available()) 
		{
			int inByte = serial1.read();
			serial0.print((char)inByte); 
		}
	}
}