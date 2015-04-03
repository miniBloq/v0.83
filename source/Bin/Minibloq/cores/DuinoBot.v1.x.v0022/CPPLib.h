#ifndef CPPLib__h
#define CPPLib__h

//"Alternate linkage specification":
extern "C"
{
	//#include <avr/io.h>
	//#include <stdlib.h>
	//#include <avr/sfr_defs.h>
	#include <stdint.h>
}


//Global overloaded new and delete operators can't be declared within a namespace:
// 'void* SNAPI::operator new(SNAPI::size_t)' may not be declared within a namespace"
typedef unsigned int size_t;
typedef void (*new_handler)();

extern "C" void __cxa_pure_virtual();
void * operator new (size_t sz) throw();
void * operator new[] (size_t sz) throw();
void operator delete (void *ptr) throw ();
void operator delete[] (void *ptr) throw ();


#endif
