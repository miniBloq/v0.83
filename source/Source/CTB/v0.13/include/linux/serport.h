/////////////////////////////////////////////////////////////////////////////
// Name:        linux/serport.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: serport.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SERIALPORT_LINUX_H
#define _WX_SERIALPORT_LINUX_H

#include "include/serportx.h"
#include <linux/serial.h>
#include <termios.h>

#define wxCOM1 "/dev/ttyS0"
#define wxCOM2 "/dev/ttyS1"
#define wxCOM3 "/dev/ttyS2"
#define wxCOM4 "/dev/ttyS3"
#define wxCOM5 "/dev/ttyS4"
#define wxCOM6 "/dev/ttyS5"
#define wxCOM7 "/dev/ttyS6"
#define wxCOM8 "/dev/ttyS7"
#define wxCOM9 "/dev/ttyS8"

/*!
   \class wxSerialPort

   \brief the linux version
*/
class wxSerialPort : public wxSerialPort_x
{
protected:
    /*!
      \brief under Linux, the serial ports are normal file descriptor
    */
    int fd;
    /*!
      \brief Linux defines this struct termios for controling asynchronous
      communication. t covered the active settings, save_t the original
      settings.
    */
    struct termios t, save_t;

    /*!
      \brief The Linux serial driver summing all breaks, framings, overruns
      and parity errors for each port during system runtime. Because we only
      need the errors during a active connection, we must save the actual
      error numbers in this separate structurs.
    */
    struct serial_icounter_struct save_info, last_info;

    /*!
      \brief adaptor member function, to convert the plattform independent
      type wxBaud into a linux conform value.
      \param baud the baudrate as wxBaud type
      \return speed_t linux specific data type, defined in termios.h
    */
    speed_t AdaptBaudrate(wxBaud baud);

    int CloseDevice();
    int OpenDevice(const char* devname, void* dcs);
public:
    wxSerialPort();
    ~wxSerialPort();

    int ChangeLineState(wxSerialLineState flags);
    int ClrLineState(wxSerialLineState flags);
    int GetLineState();
    int Ioctl(int cmd,void* args);
    int IsOpen();
    int Read(char* buf,size_t len);
    int SendBreak(int duration);
    int SetBaudRate(wxBaud baudrate);
    int SetLineState(wxSerialLineState flags);
    int Write(char* buf,size_t len);
};

#endif
