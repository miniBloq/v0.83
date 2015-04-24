/////////////////////////////////////////////////////////////////////////////
// Name:        getopt.h
// Purpose:     Simple wrapper file
// Author:      Joachim Buermann
// Id:          $Id: getopt.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_GETOPT_H
#define _WX_GETOPT_H

#if defined (WIN32)
# include "win32/getopt.h"
#else
# include <getopt.h>
#endif

#endif
// __WX_GETOPT_H
