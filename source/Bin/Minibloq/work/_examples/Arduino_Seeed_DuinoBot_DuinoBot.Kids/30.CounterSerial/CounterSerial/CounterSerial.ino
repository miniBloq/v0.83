#include <mbq.h>

void setup()
{
	initBoard();

	float count = 0;
	while(true)
	{
		serial0.println(count);
		count = (count+1);
		delay(300);
	}
}

void loop()
{
}
