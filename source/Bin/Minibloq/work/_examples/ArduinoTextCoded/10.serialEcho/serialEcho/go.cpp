#include <mbq.h>

void go()
{
	serial0.begin(115200);
	serial1.begin(115200);

	while(true)
	{
		if (serial1.available()) 
		{
			int inByte = serial1.read();
			serial1.print((char)inByte); 
		}
	}
}