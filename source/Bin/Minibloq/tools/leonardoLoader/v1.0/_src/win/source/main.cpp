/****************************************************************
    leonardoLoader

    (c) 2013 J. U. da Silva Gillig.
    For more information, please visit: http://minibloq.org
    This program is distributed under the GPL license.

    Based on the Arduino Uploader, developed by Stanley Huang,
    and distributed under the GPL license:
    https://github.com/stanleyhuangyc/ArduinoUploader
*****************************************************************/

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
 	fprintf(msgOutput, "\nUsage: leonardoLoader <avrdude> <conf file> <hex file> <serial port> [tries]\n\n");
	fprintf(msgOutput, "avrdude: complete path, including file name, of the avrdude uploader.\n");
	fprintf(msgOutput, "conf file: complete path, including file name, of the avrdude.conf file.\n");
	fprintf(msgOutput, "hex file: complete path, including file name, of the hex file to be uploaded.\n");
	fprintf(msgOutput, "port: complete name of the serial port.\n");
	fprintf(msgOutput, "tries: Number of tries to find the new serial port after reset (default = 10).\n\n");

	fprintf(msgOutput, "(c) 2013 J. U. da Silva Gillig.\nFor more information, please visit: http://minibloq.org\n");
	fprintf(msgOutput, "This program is distributed under the GPL license.\n\n");

	fprintf(msgOutput, "Based on the Arduino Uploader, ");
	fprintf(msgOutput, "developed by Stanley Huang and distributed under the GPL license: ");
	fprintf(msgOutput, "https://github.com/stanleyhuangyc/ArduinoUploader\n\n");
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

    msgOutput = stderr;
    //msgOutput = stdout;

    if (argc == 1)
    {
        help();
        return 0;
    }

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

#ifdef WIN32
    //Call to the external uploader:
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInformation;

	ZeroMemory(&processInformation, sizeof(processInformation));
	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
    //startInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    //startInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);

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
            return 20; //Error.
        }
        else
        {
            fprintf(msgOutput, "\n\nDone.\n");
            return 0; //Ok.
        }
    }
#else
    //##Add Linux code here.
    //##Future: add Mac OS X code too.
#endif

    return 0;
}
