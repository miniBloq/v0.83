#include <mbq.h>

void go()
{
	textLCD.begin(16, 2);
	textLCD.print("Hello world!");
	
	while(true)
	{
		textLCD.setCursor(0, 1);
		textLCD.print(random());
		delay(250);
	}
}
