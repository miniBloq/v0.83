//##Agregar licencias y menciones.

#include <string>
#include <vector>
#include <ctb-0.16/ctb.h>

extern "C"
{
    #include <cstdlib>
}

#ifdef WIN32
    #include <windows.h>
    #define sleep(ms)  (Sleep(ms))
#else
    #define sleep(ms)  (usleep(ms<<10))
#endif

using namespace std;


vector<string> ports;
FILE *msgOutput;


void help()
{
 	fprintf(msgOutput, "Usage: leonardoLoader <avrdude> <conf file> <hex file> <serial port> [tries]\n");
	fprintf(msgOutput, "\tavrdude: complete path, including file name, of the avrdude uploader.\n");
	fprintf(msgOutput, "\tconf file: complete path, including file name, of the avrdude.conf file.\n");
	fprintf(msgOutput, "\thex file: complete path, including file name, of the hex file to upload.\n");
	fprintf(msgOutput, "\tport: complete name of the serial port.\n");
	fprintf(msgOutput, "\ntries: Number of tries to find the new serial port after reset (default = 10).\n\n");

	fprintf(stderr, "For more information, please visit:\n");
	fprintf(msgOutput, "http://minibloq.org\n");
	fprintf(msgOutput, "(c) 2013 J. U. da Silva Gillig\n\n");

	fprintf(msgOutput, "\nBased on the Arduino Uploader (https://github.com/stanleyhuangyc/ArduinoUploader),\n");
	fprintf(msgOutput, "\nDeveloped by Stanley Huang. Distributed under GPL license.\n");
	fprintf(msgOutput, "\n");
}


string findNewPort(unsigned int tries)
{
	vector<string> newPorts;
	bool found = true;
	unsigned int k = 0;

	fprintf(msgOutput, "Searching for new ports:\n");
	while ((k < tries) && found)
	{
		newPorts.clear();
		ctb::GetAvailablePorts(newPorts);
		for(unsigned int i = 0; i < newPorts.size(); i++)
		{
            fprintf(msgOutput, "%s ", newPorts[i].c_str());
			// look for port in previous ports list
			found = false;
			for (unsigned int j = 0; j < ports.size(); j++)
			{
				if (newPorts[i].compare(ports[j]) == 0)
				{
					found = true;
					break;
				}
			}
			if (!found)
                return newPorts[i];
		}
		fprintf(msgOutput, "\n");
		newPorts.clear();
		sleep(1000);
		k++;
	}
	return "";
}


string resetBoard(const char* port, unsigned int seachTries)
{
	string uploadPort;
	ctb::SerialPort serialPort;

    fprintf(msgOutput, "\nCurrent ports:\n");
    for (unsigned int r = 0; r < ports.size(); r++)
        fprintf(msgOutput, "%s; ", ports[r].c_str());
    fprintf(msgOutput, "\n\n");

	if (serialPort.Open(port, 1200) != 0)
		return "";
	serialPort.Close();

	uploadPort = findNewPort(seachTries);
	fprintf(msgOutput, "\n");
	return string("//./") + uploadPort;
}


int main(int argc, char **argv)
{
    const char* uploader = NULL;
    const char* serial = NULL;
    const char* hexFileName = NULL;
    const char* confFileName = NULL;
    const char* tries = NULL;
    unsigned int nTries = 10; //Default number of tries for the new port search.
    string uploadPort;
    string commandLine;

    //msgOutput = stderr;
    msgOutput = stdout;

    if (argc == 0)
        help();

	//External uploader to run:
	if (argc > 1)
	{
	    uploader = argv[1];
		if (!strcmp(uploader, ""))
            uploader = NULL;
	}

    //avrdude's conf file (with complete path):
	if (argc > 2)
	{
	    confFileName = argv[2];
		if (!strcmp(confFileName, ""))
            confFileName = NULL;
	}

    //Hex file param (##no eep files supported by now):
	if (argc > 3)
	{
	    hexFileName = argv[3];
		if (!strcmp(hexFileName, ""))
            hexFileName = NULL;
	}

    //Serial port param:
	if (argc > 4)
	{
	    serial = argv[4];
		if (!strcmp(serial, ""))
            serial = NULL;
	}

	//Number of tries (for the new port search) param. This param is optional:
	if (argc > 5)
	{
	    tries = argv[5];
		if (!strcmp(tries, ""))
            tries = NULL;
        nTries = atoi(tries);
        //fprintf(msgOutput, "Tries: %u.\r\n", nTries);
	}

    //Find current ports:
	ports.clear();
    if( !ctb::GetAvailablePorts(ports))
        fprintf(msgOutput, "No serial ports found.\r\n");

    //Resets de board and search for new ports:
    fprintf(msgOutput, "\nReseting board: try to open and close port %s at 1200 bps...\n", serial);
    uploadPort = resetBoard(serial, nTries);
    if (!uploadPort.empty())
    {
        fprintf(msgOutput, "\nNew port found: %s\n", uploadPort.c_str());
    }
    else
    {
        fprintf(msgOutput, "\nNo new port found.\n");
        return 10; //Error.
    }

    //Call to the external uploader:
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInformation;


	ZeroMemory(&processInformation, sizeof(processInformation));
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);

#ifdef WIN32
    if (uploader && hexFileName)
    {
        commandLine.assign(string(""));
        commandLine =   string("\"") + string(uploader) + string("\" ") +
                        string("\"") + string(confFileName) + string("\" ") +
                        string("-patmega32u4 -cavr109 -P") + string(uploadPort) +
                        string(" -D -Uflash:w:\"") + string(hexFileName) + string("\":i");

        fprintf(msgOutput, "\nCalling external uploader: %s\n", commandLine.c_str());
        if (!CreateProcess(NULL,
                           (char *)commandLine.c_str(),
                           NULL,
                           NULL,
                           FALSE,
                           DETACHED_PROCESS,
                           NULL,
                           NULL, //Working dir.
                           &startInfo,
                           &processInformation )
            )
        {
            return 0;
        }
        else
            return 20; //Error.
    }
#else
    //##Add Linux code here.
    //##Future: add Mac OS X code too.
#endif

    return 0;
}
