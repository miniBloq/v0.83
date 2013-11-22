#include <mbq.h>

void setup()
{
	initBoard();

	float irCode = 0;
	float ultrasonic = 0;
	while(true)
	{
		irCode = ;
		toneWithDelay(BuzzerPin, (440*irCode), 200);
		if(((int)(irCode)==(int)(5)))
		{
			toneWithDelay(BuzzerPin, 440, 100);
			ultrasonic = ;
			serial0.println(ultrasonic);
		}
		else
		{
		}
	}
}

void loop()
{
}
