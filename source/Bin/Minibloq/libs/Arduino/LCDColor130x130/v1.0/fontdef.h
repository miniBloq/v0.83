/*

Font descriptor definition for Graphic class
Federico Lanza for Robotgroup

Apr, 2012

*/

#ifndef FONTDEF_H
#define FONTDEF_H

#include <stdint.h>

struct CharDescriptor
{
  uint8_t Wide;
  uint16_t CharmapStart;
};

#endif
