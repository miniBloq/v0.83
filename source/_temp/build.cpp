//##DELETE THIS!:
#if 0
//##Esto será también configurable para cada target (porque el reset podría ocurrir de muchas formas
//diferentes, o sobre señales distintas de los ports):
bool Bubble::resetBoard()
{
    if (getBoardName() == wxString("ATTiny25 (with ArduinoISP)") ||
        getBoardName() == wxString("ATTiny45 (with ArduinoISP)") ||
        getBoardName() == wxString("ATTiny85 (with ArduinoISP)") ||
        getBoardName() == wxString("ATTiny25 (with Doper)") ||
        getBoardName() == wxString("ATTiny45 (with Doper)") ||
        getBoardName() == wxString("ATTiny85 (with Doper)")
       )
    {
        return true;
    }

    if (!bootSerialPort.IsOpen())
		bootSerialPort.Open(bootPortName.char_str());
    if (bootSerialPort.IsOpen()) //This is NOT the same as en "else"!
    {
        if (getBoardName() == wxString("DuinoBot.v1.x") || //##Un-hardcode:
            getBoardName() == wxString("DuinoBot.Kids.v1.x") ) //##Un-hardcode:
        {
            bootSerialPort.SetLineState(wxSERIAL_LINESTATE_DTR);
            wxMilliSleep(100); //##Make this configurable.
            bootSerialPort.ClrLineState(wxSERIAL_LINESTATE_DTR);
        }
        else
        {
            //##Standard Arduino:
            bootSerialPort.SetLineState(wxSERIAL_LINESTATE_DTR);
            wxMilliSleep(10); //##Make this configurable.
            bootSerialPort.ClrLineState(wxSERIAL_LINESTATE_DTR);
            //wxMilliSleep(5); //##Make this configurable.
            //bootSerialPort.SetLineState(wxSERIAL_LINESTATE_DTR);
        }
        bootSerialPort.Close();
        return true;
    }
	return false;
}
#endif


//##DELETE THIS!:
#if 0
bool Bubble::build()
{
    //##Profiling:
    wxLongLong millis = wxGetLocalTimeMillis();

    //First, reset the progress bar:
    if (getNotifier())
    {
        //##getNotifier()->setProgressPosition(0, false, false);
        getNotifier()->clearMessage();
    }

    if (getBoardName() == wxString("DuinoBot.v1.x.HID")) //##Un-hardcode.
    {
        //##Optimización futura:
        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA/Scheduler\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            wxArrayString temp; //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Generates the .cpp files for the other blocks:
            //##IRRemote.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/DuinoBot_Motor.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Near future: The .cpp from the blocks must come from the arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Servo.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x.HID_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/DuinoBot_Motor.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources/Servo.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //##Ver cómo se hará en el futuro para que no haya conflicto con las librerías que hubo que instalar en los cores debido a la falta
            //de HAL real en Arduino...

            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega32u4 -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##

                    //##Archivos de las librerías de los bloques (deben salir de la enumeración):
                    //##Conflicto de timers con Tone:
                    /////////////
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    /////////////

                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\" ") + //##

                    //wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##

                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=atmega32u4 --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }
    else if (getBoardName() == wxString("DuinoBot.v1.x")) //##Un-hardcode.
    {
        //##Optimización futura:
        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            wxArrayString temp; //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Generates the .cpp files for the other blocks:
            //##IRRemote.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/DuinoBot_Motor.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Near future: The .cpp from the blocks must come from the arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Servo.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.v1.x_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/DuinoBot_Motor.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources/Servo.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //##Ver cómo se hará en el futuro para que no haya conflicto con las librerías que hubo que instalar en los cores debido a la falta
            //de HAL real en Arduino...

            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega32u4 -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##

                    //##Archivos de las librerías de los bloques (deben salir de la enumeración):
                    //##Conflicto de timers con Tone:
                    /////////////
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    /////////////

                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\" ") + //##

                    //wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##

                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=atmega32u4 --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }
    else if (getBoardName() == wxString("DuinoBot.Kids.v1.x")) //##Un-hardcode.
    {
        //##Optimización futura:
        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    //##wxString("-I\"") + libPath + wxString("/CPP/Blocks/DuinoBot_Motor.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            wxArrayString temp; //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Genera los archivos de los .cpp de los demás bloques:
            //##IRRemote.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/DuinoBot_Motor.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //##Servo.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/DCMotor\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA/Common\" ") +
                    wxString("-I\"") + corePath + wxString("/DuinoBot.Kids.v1.x_0022/LUFA/Drivers\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/DuinoBot_Motor.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources/Servo.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega32u4 -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##

                    //##Archivos de las librerías de los bloques (deben salir de la enumeración):
                    //##Conflicto de timers con Tone:
                    /////////////
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    /////////////

                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\" ") + //##

                    //##wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##

                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=atmega32u4 --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }
    else if (   (getBoardName() == wxString("Arduino Uno")) ||          //##Un-hardcode.
                (getBoardName() == wxString("Arduino Duemilanove Mega328")) ||  //##Un-hardcode.
                (getBoardName() == wxString("Seeeduino v2.2x Mega328")) //##Un-hardcode.
            )
    {
        //##Optimización futura:
        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //##Motor.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoUNO_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources/DCMotor.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\"");
            wxArrayString temp; //##
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Generates the .cpp files for the other blocks:
            //##IRRemote.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoUNO_0022\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Near future: The .cpp from the blocks must come from the arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Servo.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoUNO_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources/Servo.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega328p -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoUNO_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            //wxArrayString temp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file from the ".o" and ".a" files:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega328p -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=atmega328p --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }
    else if (   (getBoardName() == wxString("Seeeduino Mega 1280")) ||
                (getBoardName() == wxString("Arduino Mega 1280"))
            )
    {
        //##Optimización futura:
        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //##Motor.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega1280 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega1280_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources/DCMotor.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\"");
            wxArrayString temp; //##
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Generates the .cpp files for the other blocks:
            //##IRRemote.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega1280 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega1280_0022\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Near future: The .cpp from the blocks must come from the arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Servo.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega1280 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega1280_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources/Servo.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega1280 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega1280_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            //wxArrayString temp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file from the ".o" and ".a" files:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega1280 -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=atmega1280 --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }
    else if (getBoardName() == wxString("Arduino Mega 2560"))
    {
        //##Optimización futura:
        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //##Motor.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega2560_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources/DCMotor.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\"");
            wxArrayString temp; //##
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Generates the .cpp files for the other blocks:
            //##IRRemote.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega2560_0022\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Near future: The .cpp from the blocks must come from the arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Servo.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega2560_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources/Servo.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoMega2560_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            //wxArrayString temp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file from the ".o" and ".a" files:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega2560 -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=atmega2560 --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }
    else if (getBoardName() == wxString("Arduino Duemilanove Mega168"))
    {
        //##Optimización futura:
        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //##Motor.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega168 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoM168_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources/DCMotor.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\"");
            wxArrayString temp; //##
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Generates the .cpp files for the other blocks:
            //##IRRemote.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega168 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoM168_0022\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Near future: The .cpp from the blocks must come from the arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //##Servo.cpp:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega168 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoM168_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    //##Archivos .cpp de los bloques, deben salir de los arrays:
                    wxString("\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources/Servo.cpp") + wxString("\" ") +
                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\"");
            temp.Clear(); //##
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            //##Ver cómo hacer para mostrar los errores mejor, con doble click (que va a la ventana de código
            //generado, porque luego estarán los errores de bloques, en otra ventana de mensajes, o en la misma), etc...
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu=atmega168 -DF_CPU=16000000L -DARDUINO=22 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + corePath + wxString("/ArduinoM168_0022\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0070.ServoRC.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            //wxArrayString temp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file from the ".o" and ".a" files:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=atmega168 -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + outputPath + wxString("/Servo.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=atmega168 --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }
    else if (   getBoardName() == wxString("ATTiny25 (with ArduinoISP)") ||
                getBoardName() == wxString("ATTiny45 (with ArduinoISP)") ||
                getBoardName() == wxString("ATTiny85 (with ArduinoISP)") ||
                getBoardName() == wxString("ATTiny25 (with Doper)") ||
                getBoardName() == wxString("ATTiny45 (with Doper)") ||
                getBoardName() == wxString("ATTiny85 (with Doper)")
            )
    {
        //##Optimización futura:
        wxString mcu("");
        if (getBoardName() == wxString("ATTiny25 (with Doper)") ||
            getBoardName() == wxString("ATTiny25 (with ArduinoISP)"))
            mcu = "attiny25";
        else if (   getBoardName() == wxString("ATTiny45 (with Doper)") ||
                    getBoardName() == wxString("ATTiny45 (with ArduinoISP)"))
            mcu = "attiny45";
        else if (   getBoardName() == wxString("ATTiny85 (with Doper)") ||
                    getBoardName() == wxString("ATTiny85 (with ArduinoISP)"))
            mcu = "attiny85";

        //Esta secuencia de llamadas genera código ligeramente mayor al generado por el IDE 0021 de Arduino.
        if (generateCodeAndSaveToFile())
        {
            //##Terminar de implementar:

            wxArrayString output, errors;
            wxString cmd("");

            //##Generates the .cpp files for the other blocks:
            ////##IRRemote.cpp:
            //cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu="+
            //        mcu + " -DF_CPU=1000000L -DARDUINO=18 ") +
            //        //##Este path va a cambiar, ya que el core va a estar incluído en dirs de Minibloq:
            //        wxString("-I\"") + corePath + wxString("\" ") +
            //        wxString("-I\"") + corePath + wxString("/Tiny25.45.85_0018\" ") +
            //        wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
            //
            //        //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +
            //
            //        //##Near future: The .cpp from the blocks must come from the arrays:
            //        wxString("\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources/IRremote.cpp") + wxString("\" ") +
            //
            //        wxString("-o") +
            //        wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\"");
            //

            wxArrayString temp;
            //temp.Clear(); //##
            //temp.Add(cmd);
            //showStream(temp); //##
            //wxExecute(cmd, output, errors);
            //if (findErrorStringAndShow(errors))
            //    return false;

            //Generates the main.cpp.o file:
            cmd =   toolsPath + wxString("/avr-g++ -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -mmcu="+
                    mcu + " -DF_CPU=1000000L -DARDUINO=18 ") +

                    //##Estos son los directorios de include que luego hay que descablear, obteniéndolos de los bloques, que los
                    //mandarán a un array (habrá 2 arrays: uno de "Includes" y otro de "IncludePaths", o mejor: uno de Libraries, y
                    //otro de LibraryPaths, o algo así):
                    wxString("-I\"") + corePath + wxString("/Tiny25.45.85_0018\" ") +
                    wxString("-I\"") + corePath + wxString("\" ") +
                    wxString("-I\"") + componentPath + wxString("\" ") +
                    wxString("-I\"") + outputPath + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/Arduino_IRRemote.v1.0/Sources") + wxString("\" ") +
                    //##Ver cómo se hará en el futuro, para que no conflictúen bloques como el de motor, que pueden estar incluídos en un
                    //core de Arduino, como el de DuinoBot o el DuinoBot.Kids:
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0060.Motor.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/action.020.0050.Buzzer.Arduino.v1.0/Sources") + wxString("\" ") +
                    //wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0010.IRRemote.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0020.Ping.Arduino.v1.0/Sources") + wxString("\" ") +
                    wxString("-I\"") + libPath + wxString("/CPP/Blocks/number.200.0030.IRRanger.Arduino.v1.0/Sources") + wxString("\" ") +

                    //##2011.03.07:wxString("\"") + componentPath + wxString("/main.cpp") + wxString("\"") +

                    wxString("\"") + corePath + wxString("/main.cpp") + wxString("\" ") +

                    wxString("-o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\"");
            //wxArrayString temp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.elf file from the ".o" and ".a" files:
            cmd =   toolsPath + wxString("/avr-gcc -Os -Wl,--gc-sections -mmcu=" + mcu + " -o") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +  //##
                    wxString("\"") + outputPath + wxString("/main.cpp.o") + wxString("\" ") + //##
                    //wxString("\"") + outputPath + wxString("/DCMotor.cpp.o") + wxString("\" ") + //##
                    //wxString("\"") + outputPath + wxString("/IRremote.cpp.o") + wxString("\" ") + //##
                    wxString("\"") + corePath + wxString("/core.a") + wxString("\"") +
                    wxString(" -L .\\ -lm");
            //wxArrayString tmp; //##
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.epp file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.epp") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //Generates the main.cpp.hex file:
            cmd =   toolsPath + wxString("/avr-objcopy -O ihex -R .eeprom ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\" ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.hex") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            if (findErrorStringAndShow(errors))
                return false;

            //
            cmd =   toolsPath + wxString("/avr-size --mcu=" + mcu + " --format=avr ") +
                    wxString("\"") + outputPath + wxString("/main.cpp.elf") + wxString("\"");
            temp.Clear();
            temp.Add(cmd);
            showStream(temp); //##
            wxExecute(cmd, output, errors);
            showStream(output); //##Bug: Ver por qué muestra availabl...

            //##Profiling: This may be disabled by the user in the future:
            long millisResult = 0;
            millisResult = wxGetLocalTimeMillis().ToLong() - millis.ToLong();
            getNotifier()->showMessage((_("milliseconds: ") + (wxString("") << millisResult)) + wxString("\n\n"), false, false, *wxWHITE); //##Debug

            //##Ver qué se hace con el retorno y el control de errores:
            return true;
        }
        return false;
    }

    return false;
}
#endif


//##DELETE THIS!:
#if 0
bool Bubble::deploy()
{
    if (getNotifier() == NULL) //##Ver si esto se dejará así, o si se mejorará en el futuro...
        return false;

    if (build())
    {
        //##En el futuro, para optimizar, existirá la opción de resetear antes del build() en los targets
        //que así se especifique (ya que en boards como Arduino, eso no funcionaría, por el time-out del
        //bootloader de Arduino, pero en los boards como DuinoBot.Kids, eso ganaría bastante tiempo):
        getNotifier()->showMessage(_("Reseting the board...\n"), false, false, *wxBLUE);
        getNotifier()->deployStartedNotify();
        resetBoard(); //##Futuro: Usar el retorno booleano de resetBoard...

        //Not necessary: Everything is managed by the hid_bootloader_cli program.
        //if (getBoardName() == wxString("DuinoBot.v1.x.HID"))
        //{
        //}

        if (getBoardName() == wxString("DuinoBot.v1.x") || //##Un-hardcode:
            getBoardName() == wxString("DuinoBot.Kids.v1.x") ) //##Un-hardcode:
        {
            //First, waits until the port desappears:
            int times = 0;

            //##En las placas con ATMega32U4, primero el puerto tiene que desaparecer tras el reset, y una vez que
            //desapareció, recién se verifica (en el while que sigue a éste) que el puerto exista nuevamente:

            //##Hacer la cantidad de iteraciones configurable:
            times = 0;
            while ( (BubbleHardwareManager::serialPortExists(bootPortName)) && (times < 10) )
            {
                times++;
                getNotifier()->showMessage(_(">"), false, false, *wxBLUE);
                wxMilliSleep(200); //##Hacer esto configurable.
            }
            //getNotifier()->showMessage(_("\n"), false, false, *wxBLUE);

            //Waits until the port does exist, but with a timeout:
            getNotifier()->showMessage(_("\nVerifying port ") + bootPortName, false, false, *wxBLUE);
            //##Hacer la cantidad de iteraciones configurable:
            times = 0;
            while ( (!BubbleHardwareManager::serialPortExists(bootPortName)) && (times < 10) )
            {
                times++;
                getNotifier()->showMessage(_(">"), false, false, *wxBLUE);
                wxMilliSleep(200); //##Hacer esto configurable.
            }
            getNotifier()->showMessage(_("\n\n"), false, false, *wxBLUE);
        }
        else if (getBoardName() == wxString("DuinoBot.v1.x.HID"))
        {
            wxString cmd = wxString("");
#if defined (WIN32)
            cmd =   getComponentsRepositoryPath() + wxString("/tools/hidUploader/v1.0/hid_bootloader_cli.exe ") + //##
#else
            cmd =   getComponentsRepositoryPath() + wxString("/tools/hidUploader/v1.0/hid_bootloader_cli.Linux ") + //##
#endif
                    wxString("-mmcu=atmega32u4 -r -v \"") +
                    outputPath + wxString("/main.cpp.hex\"");

            getNotifier()->showMessage(_("\nBoard Ok >> Programming...\n\n"), false, false, *wxBLUE);
            wxArrayString output, errors;
            wxArrayString temp;
            temp.Add(cmd);
            showStream(temp);
            wxExecute(cmd, output, errors);
            findErrorStringAndShow(errors);
        }
        else
        {
            //##I think it will be better to not use the "-D" option in avrdude:
            //The port may not exist at the time the user calls the delploy() function, so this prevents the
            //program to hang for some time (which may be a lot of time) when this happens:
            if (BubbleHardwareManager::serialPortExists(bootPortName) || //This is more secure than flaging the port existence inside the timeout while.
                getBoardName() == wxString("ATTiny25 (with ArduinoISP)") ||
                getBoardName() == wxString("ATTiny45 (with ArduinoISP)") ||
                getBoardName() == wxString("ATTiny85 (with ArduinoISP)") ||
                getBoardName() == wxString("ATTiny25 (with Doper)") ||
                getBoardName() == wxString("ATTiny45 (with Doper)") ||
                getBoardName() == wxString("ATTiny85 (with Doper)")
               )
            {
                wxString cmd = wxString("");
                if (getBoardName() == wxString("DuinoBot.v1.x") ||      //##Un-hardcode:
                    getBoardName() == wxString("DuinoBot.Kids.v1.x") )  //##Un-hardcode:
                {
                    //##En el futuro, puede que sea mejor implementar al revés la obtención del serialPortName, de modo
                    //que el deploy lo pida directamente al HardwareManager cada vez que es invocado. Esto ahorraría
                    //eventuales problemas de actualización, que dependen del evento del combo (o del selector que se
                    //esté utilizando) del HardwareManager:
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20090314.exe -C\"") +
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                            wxString(" -patmega32u4 -cavr109") +
                            //wxString(" -P//./COM48") + //##Serial port number = Make it configurable.
                            wxString(" -P") + bootPortName + //##Serial port number = Make it configurable.
                            //wxString(" -b115200 -D -Uflash:w:\"") +
                            wxString(" -b115200 -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");
                //        wxMessageDialog dialog0(parent, cmd, _("Debug:")); //##Debug.
                //        dialog0.ShowModal(); //##Debug.

                    //executeCmd(cmd);
                }
                else if (getBoardName() == wxString("Seeeduino v2.2x Mega328") ) //##Un-hardcode.
                {
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                            wxString(" -patmega328p -cstk500v1") +
                            wxString(" -P") + bootPortName +
                            //wxString(" -b57600 -D -Uflash:w:\"") +
                            wxString(" -b57600 -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");
                }
                else if (getBoardName() == wxString("Arduino Uno"))
                {
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -F -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                            wxString(" -patmega328p -cstk500v1") +
                            wxString(" -P") + bootPortName +
                            //wxString(" -b115200 -D -Uflash:w:\"") +
                            wxString(" -b115200 -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");
                    //##Esta línea es valiosa, porque escribe independientemente de la signatura. Arduino UNO y el avrdude
                    //del WinAVR-20090313, por ejemplo, se llevan mal, y el mismo comando que en el avrdude que viene con
                    //el ArduinoIDE-0022 (WinAVR-20081205) sí funciona bien. Así que es probable que deje por defecto el "-F",
                    //aunque por ahora no, porque puede que esto proteja contra malas configuraciones por parte de terceros
                    //que creen targets:
                    //wxString cmd =  toolsPath + wxString("/avrdude.exe -F -C\"") +//##
                    //wxString cmd =  toolsPath + wxString("/avrdude.20071012.exe -F -C\"") +//##
                }
                else if (getBoardName() == wxString("Arduino Mega 2560"))
                {
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                            wxString(" -patmega2560 -cstk500v2") +
                            wxString(" -P") + bootPortName +
                            //wxString(" -b115200 -D -Uflash:w:\"") +
                            wxString(" -b115200 -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");
                }
                else if (   (getBoardName() == wxString("Seeeduino Mega 1280")) ||
                            (getBoardName() == wxString("Arduino Mega 1280"))
                        )
                {
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                            wxString(" -patmega1280 -cstk500v1") +
                            wxString(" -P") + bootPortName +
                            //wxString(" -b57600 -D -Uflash:w:\"") +
                            wxString(" -b57600 -Uflash:w:\"") + //##This must be user-configurable.
                            outputPath + wxString("/main.cpp.hex\":i");
                }
                else if (getBoardName() == wxString("Arduino Duemilanove Mega328"))
                {
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                            wxString(" -patmega328p -cstk500v1") +
                            wxString(" -P") + bootPortName +
                            //wxString(" -b57600 -D -Uflash:w:\"") +
                            wxString(" -b57600 -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");
                }
                else if (getBoardName() == wxString("Arduino Duemilanove Mega168"))
                {
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                            wxString(" -patmega168 -cstk500v1") +
                            wxString(" -P") + bootPortName +
                            //wxString(" -b57600 -D -Uflash:w:\"") +
                            wxString(" -b19200 -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");
                }
                else if (   getBoardName() == wxString("ATTiny25 (with ArduinoISP)") ||
                            getBoardName() == wxString("ATTiny45 (with ArduinoISP)") ||
                            getBoardName() == wxString("ATTiny85 (with ArduinoISP)")
                        )
                {
                    wxString mcu("");
                    if (getBoardName() == wxString("ATTiny25 (with ArduinoISP)"))
                        mcu = "attiny25";
                    else if (getBoardName() == wxString("ATTiny45 (with ArduinoISP)"))
                        mcu = "attiny45";
                    else if (getBoardName() == wxString("ATTiny85 (with ArduinoISP)"))
                        mcu = "attiny85";
                    //ArduinoISP:
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            //toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
                            toolsPath + wxString("/avrdude.conf\"") +
                            wxString(" -p " + mcu + " -c stk500v1 -b19200 ") +
                            wxString(" -P") + bootPortName +
                            wxString(" -V -F -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");
                }
                else if (   getBoardName() == wxString("ATTiny25 (with Doper)") ||
                            getBoardName() == wxString("ATTiny45 (with Doper)") ||
                            getBoardName() == wxString("ATTiny85 (with Doper)")
                        )
                {
                    wxString mcu("");
                    if (getBoardName() == wxString("ATTiny25 (with Doper)"))
                        mcu = "attiny25";
                    else if (getBoardName() == wxString("ATTiny45 (with Doper)"))
                        mcu = "attiny45";
                    else if (getBoardName() == wxString("ATTiny85 (with Doper)"))
                        mcu = "attiny85";
                    //##With AVRDoper:
                    //.\avrdude -C .\avrdude.conf -p attiny25 -c stk500v2 -P avrdoper -V -U flash:w:".\LineFollower0.cpp.hex":i
#if defined (WIN32)
                    cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
#else
                    cmd =   toolsPath + wxString("/avrdude -C\"") +//##
#endif
                            toolsPath + wxString("/avrdude.conf\"") +
                            wxString(" -p " + mcu + " -c stk500v2 -P avrdoper") +
                            wxString(" -V -F -Uflash:w:\"") +
                            outputPath + wxString("/main.cpp.hex\":i");

                    //With USBasp:
                    //cmd =   toolsPath + wxString("/avrdude.20071012.exe -C\"") +//##
                    //        toolsPath + wxString("/avrdude.conf") + wxString("\"") +
                    //        wxString(" -p " + mcu + " -c usbasp") +
                    //        wxString(" -V -F -Uflash:w:\"") +
                    //        outputPath + wxString("/main.cpp.hex\":i");
                }
                getNotifier()->showMessage(_("Verifiying the board ") + getBoardName() + wxString("\n"), false, false, *wxBLUE);

                if (getBoardName() == wxString("ATTiny25 (with ArduinoISP)") ||
                    getBoardName() == wxString("ATTiny45 (with ArduinoISP)") ||
                    getBoardName() == wxString("ATTiny85 (with ArduinoISP)") ||
                    getBoardName() == wxString("ATTiny25 (with Doper)") ||
                    getBoardName() == wxString("ATTiny45 (with Doper)") ||
                    getBoardName() == wxString("ATTiny85 (with Doper)")
                   )
                {
                    getNotifier()->showMessage(_("\nBoard Ok >> Programming...\n\n"), false, false, *wxBLUE);

                    wxArrayString output, errors;
                    //int code = wxExecute(cmd, output, errors);
                    wxArrayString temp; //##
                    temp.Add(cmd);
                    showStream(temp); //##
                    wxExecute(cmd, output, errors);
                    findErrorStringAndShow(errors); //##Debug
                }
                else
                {
                    if (verifyBoard())
                    {
                        getNotifier()->showMessage(_("\nBoard Ok >> Programming...\n\n"), false, false, *wxBLUE);
                        wxArrayString output, errors;
                        //int code = wxExecute(cmd, output, errors);
                        wxArrayString temp; //##
                        temp.Add(cmd);
                        showStream(temp); //##
                        wxExecute(cmd, output, errors);
                        findErrorStringAndShow(errors); //##Debug
                    }
                    else
                    {
                        getNotifier()->showMessage(_("\nError: Can't communicate with the board.\nTry reset it manually.") + bootPortName, false, true, *wxRED);
                    }
                }
            }
            else
            {
                if (getNotifier())
                {
                    getNotifier()->showMessage(_("\nCan't open port ") + bootPortName, false, true, *wxRED);
                }
            }
        }
    }

    return false;
}
#endif
