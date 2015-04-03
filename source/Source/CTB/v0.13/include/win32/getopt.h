/////////////////////////////////////////////////////////////////////////////
// Name:        win32/getopt.h
// Purpose:
// Author:      Joachim Buermann
// Id:          $Id: getopt.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

extern char* optarg;
extern int optind;

int getopt(int argc, char* argv[], char* optstring);
