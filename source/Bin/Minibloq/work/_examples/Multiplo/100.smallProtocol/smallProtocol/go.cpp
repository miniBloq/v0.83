#include <mbq.h>

#define debug 0
#if (debug)
	//debug (USB serial):
	#define baudrate 115200
	#define serial serial0
#else
	//Multiplo robots (N6, N8, Ta-Bot...):
	#define baudrate 9600
	#define serial serial1
#endif

void stopAll()
{
  motor0.setPower(0);
  motor1.setPower(0);
  noTone(BuzzerPin);
}


String getCommand()
{
	int readByte = 0;
	String command = "";

	do
	{
		if (serial.available()) 
		{
			readByte = serial.read();
			//serial.write(readByte); //Debug: echo on
			command = command + (char)readByte;
		}
	} while ((readByte != 13) && (readByte != 10)); //Any kind of line end or CR char marks the end of the current command.

	//Normalizes the command:
	//String strResult(command.toLowerCase());
	command.trim();
	command.toLowerCase();
	return command;
}


//Affects the command input string writing to it its own tail:
bool getIntParam(int &result, String& command, int min, int max)
{
	command = command.trim();
	String aux = command;
	String number = "";
	int i = 0;
	int len = command.length();

	//Get the sign:
	if (i <= len)
	{
		if ( (command[i] == '-') || (command[i] == '+') )
		{
			number = number + command[i];
			i++;
		}
	}

	//Get the number itself:
	while (i <= len)
	{
		if ( ('0' <= command[i]) && (command[i] <= '9') )
			number = number + command[i];
		else
			break;   
		i++;
	}

	//  if (i == 0)
	//  {
	//     //No number found: command and result keeps the same:
	//     return false;
	//  }

	//Numerical result:
	const int chrBufLen = 32;
	char chrBuf[chrBufLen];
	number.toCharArray(chrBuf, chrBufLen);
	//  result = atoi(chrBuf);
	long int tempResult = strtol(chrBuf, NULL, 10);
	if (tempResult > max)
		tempResult = max;
	if (tempResult < min)
		tempResult = min;
	result = (int)tempResult;

	//Tail result:
	if (i == len)
	{
		command = "";
	}
	else
	{
		command = aux.substring(i); //Returns the command's tail
		command = command.trim();
	}

	//Success!
	return true;
}


void parseCommand(const String& command)
{
	String returnString = "";
	String tail = "";
	String opcode = command.substring(0, command.indexOf(" "));
	if (opcode == "")
		opcode = command.substring(0, command.indexOf("\n"));
	if (opcode == "")
		opcode = command.substring(0, command.indexOf("\r"));
	tail = command.substring(command.indexOf(" "));
	if (tail == "")
		tail = command.substring(0, command.indexOf("\n"));
	if (tail == "")
		tail = command.substring(0, command.indexOf("\r"));

	//tail.trim();

	//Debug:
	//serial.print("opcode = ");
	//serial.println(opcode);
	//serial.print("command = ");
	//serial.println(tail);

	//serial.println(opcode); //debug.
	if (opcode.trim() == "motors")
	{
		//serial.println(opcode); //debug.
		int power0 = 0, power1 = 0;
		//serial.println(tail.trim()); //##debug.
		if (getIntParam(power0, tail, -100, 100))
		{
			getIntParam(power1, tail, -100, 100);
		}
		motor0.setPower(power1);
		motor1.setPower(power0);
		returnString = "["+ opcode + " " + power0 + " " + power1 + " " + millis() + "]";
		serial.println(returnString);
	}
	else if (opcode.trim() == "tone")
	{
		int freq = 0, duration = 0;
		if (getIntParam(freq, tail, 0, 10000))
		{
			getIntParam(duration, tail, 0, 10000);
		}
		if (freq != 0)
			tone(BuzzerPin, freq, duration);
		else
			noTone(BuzzerPin);
		returnString = "["+ opcode + " " + freq + " " + duration + " " + millis() + "]";
		serial.println(returnString);
	}
	else if (opcode.trim() == "stop")
	{
		stopAll();
		returnString = "["+ opcode + " " + millis() + "]";
		serial.println(returnString);
	}
	else if (opcode == "")
	{
	}
}


void go()
{
	String command = "";
	serial.begin(baudrate);

	motor1.setClockwise(false);

	//Tells the user that the interpreter is running:  
	tone(BuzzerPin, 440, 300);
	delay(300);
	tone(BuzzerPin, 880, 500);

	//serial.println("Robot protocol v0.1");

	//Main application cycle (more eficiente than using the loop function):
	while(true)
	{
		command = getCommand();
	    //serial.println(command); //##debug.
		parseCommand(command);
	}
}