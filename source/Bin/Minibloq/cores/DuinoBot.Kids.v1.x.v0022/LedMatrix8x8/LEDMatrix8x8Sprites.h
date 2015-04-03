#ifndef LEDMatrix8x8Sprites__h
#define LEDMatrix8x8Sprites__h

const prog_uint8_t PROGMEM invader0[] =
{
    0x18,    // ___XX___
    0x3C,    // __XXXX__
    0x7E,    // _XXXXXX_
    0xDB,    // X_XXXX_X
    0xFF,    // XXXXXXXX
    0x24,    // __X__X__
    0x5A,    // _X_XX_X_
    0xA5     // X_X__X_X
};

const prog_uint8_t PROGMEM invader1[] =
{
    0x18,    // ___XX___
    0x3C,    // __XXXX__
    0x7E,    // _XXXXXX_
    0xDB,    // X_XXXX_X
    0xFF,    // XXXXXXXX
    0x24,    // __X__X__
    0x42,    // _X____X_
    0x24     // __X__X__
};

const prog_uint8_t PROGMEM invader2[] =
{
    0x24,    // __X__X__
    0x7E,    // _XXXXXX_
    0xDB,    // XX_XX_XX
    0xFF,    // XXXXXXXX
    0xA5,    // X_X__X_X
    0x99,    // X__XX__X
    0x81,    // X______X
    0xC3     // XX____XX
};

const prog_uint8_t PROGMEM invader3[] =
{
    0x24,    // __X__X__
    0x18,    // ___XX___
    0x7E,    // X_XXXX_X
    0xDB,    // XX_XX_XX
    0xFF,    // XXXXXXXX
    0xDB,    // X_XXXX_X
    0x99,    // X__XX__X
    0xC3     // XX____XX
};

const prog_uint8_t PROGMEM heartSmall[] =
{
    0x00,    // ________
    0x00,    // ________
    0x14,    // ___X_X__
    0x3E,    // __XXXXX_
    0x3E,    // __XXXXX_
    0x1C,    // ___XXX__
    0x08,    // ____X___
    0x00     // ________
};

const prog_uint8_t PROGMEM heartBig[] =
{
    0x00,    // ________
    0x66,    // _XX__XX_
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0xFF,    // XXXXXXXX
    0x7E,    // _XXXXXX_
    0x3C,    // __XXXX__
    0x18     // ___XX___
};

const prog_uint8_t PROGMEM smile[] =
{
    0b00000000, // ________
    0b00000000, // ________
    0b01100110, // _XX__XX_
    0b01100110, // _XX__XX_
    0b00000000, // ________
    0b01000010, // _X____X_
    0b00111100, // __XXXX__
    0b00000000  // ________
};

const prog_uint8_t PROGMEM angry[] =
{
    0b00000000, // ________
    0b00000000, // ________
    0b01100110, // _XX__XX_
    0b01100110, // _XX__XX_
    0b00000000, // ________
    0b00000000, // ________
    0b00111100, // __XXXX__
    0b01000010  // _X____X_
};

const prog_uint8_t PROGMEM whatever[] =
{
    0b00000000, // ________
    0b00000000, // ________
    0b01100110, // _XX__XX_
    0b01100110, // _XX__XX_
    0b00000000, // ________
    0b00000000, // ________
    0b00111100, // __XXXX__
    0b00000000  // ________
};

#endif
