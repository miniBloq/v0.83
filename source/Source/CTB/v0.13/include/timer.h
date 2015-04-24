/////////////////////////////////////////////////////////////////////////////
// Name:        timer.h
// Purpose:     simple wrapper file
// Author:      Joachim Buermann
// Id:          $Id: timer.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_TIMER_BASE_H
#define _WX_TIMER_BASE_H

#if defined (WIN32)
# include "win32/timer.h"
#else
# include "linux/timer.h"
#endif

#endif
// __TIMER_BASE_H

