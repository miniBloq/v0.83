/////////////////////////////////////////////////////////////////////////////
// Name:        fifo.cpp
// Purpose:
// Author:      Joachim Buermann, Michael Hungershausen
// Id:          $Id$
// Copyright:   (c) 2006,2007 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "include/fifo.h"

fifo::fifo(size_t size) :
    m_size(size)
{
    m_begin = new char[size];
    m_end = m_begin + m_size;
    m_rdptr = m_wrptr = m_begin;
};

fifo::~fifo()
{
    delete m_begin;
};

void fifo::clear()
{
    m_rdptr = m_wrptr = m_begin;
};

int fifo::get(char* ch)
{
    if(m_rdptr != m_wrptr) {
	   *ch = *m_rdptr++;
	   if(m_rdptr >= m_end) {
		  m_rdptr = m_begin;
	   }
	   return 1;
    }
    return 0;
};

size_t fifo::items()
{
    char* tmp_wrptr = m_wrptr;
    // the rdptr will only changed by the reader. If we suppose, that
    // the caller of the items() method is identical with the reader,
    // this should be thread save.
    char* tmp_rdptr = m_rdptr;

    // if the write pointer is identical with the read, there are no
    // more data in the fifo
    if(tmp_wrptr == tmp_rdptr) {
	   return 0;
    }
    // the write pointer is greater as the read pointer, so we just
    // can calculate the difference for the remaining data
    if(tmp_wrptr > tmp_rdptr) {
	   return (tmp_wrptr - tmp_rdptr);
    }
    // the write pointer has circulate and stands against the read
    // pointer
    else {
	   return (m_size - (tmp_rdptr - tmp_wrptr));
    }
};

int fifo::put(char ch)
{
    // for a thread safe operation, the write of a data byte must be
    // atomic. So we first assign the current position of the write
    // pointer to a temporary pointer.
    // Increment it for the comparison with the end of the internal
    // buffer and the read pointer
    char* tmp_wrptr = m_wrptr + 1;
    if(tmp_wrptr >= m_end) {
	   tmp_wrptr = m_begin;
    }
    if(tmp_wrptr == m_rdptr) {
	   return 0;
    }
    // this don't changes the write pointer!
    *m_wrptr = ch;
    // that's the trick! The following assignment is atomic and cannot
    // interrupted within a read access by the read thread
    m_wrptr = tmp_wrptr;
    return 1;
};

int fifo::read(char* data,int n)
{
    int nresult = 0;
    while(n--) {
	   // the same as get()
	   if(m_rdptr != m_wrptr) {
		  *data = *m_rdptr++;
		  if(m_rdptr >= m_end) {
			 m_rdptr = m_begin;
		  }
	   }
	   else {
		  break;
	   }
	   nresult++;
	   data++;
    }
    return nresult;
};

int fifo::write(char* data,int n)
{
    int nresult = 0;
    while(n--) {
	   // the same as put()
	   char* tmp_wrptr = m_wrptr + 1;
	   if(tmp_wrptr >= m_end) {
		  tmp_wrptr = m_begin;
	   }
	   if(tmp_wrptr == m_rdptr) {
		  break;
	   }
	   *m_wrptr = *data++;
	   m_wrptr = tmp_wrptr;
	   nresult++;
    }
    return nresult;
};

