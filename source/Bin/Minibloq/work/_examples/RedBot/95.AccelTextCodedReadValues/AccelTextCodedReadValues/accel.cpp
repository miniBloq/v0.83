#include <RedBot.h>
#include <mbq.h>

void go()
{
	serial0.begin(115200);

	while(true)
	{
		accel.read();
		serial0.print("x=");
		serial0.print(accel.x);
		serial0.print("    y=");
		serial0.print(accel.y);
		serial0.print("    z=");
		serial0.println(accel.z);
		delay(200);
	}
}
