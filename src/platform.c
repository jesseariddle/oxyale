#include "platform.h"

uint8_t
OPAL_PlatformIsBigEndian( )
{
    union {
        uint32_t ui32;
        uint8_t ui8[4];
    } bint = {0x01020304};

    return bint.ui8[0] == 1; 
}

uint32_t
OPAL_SwapEndiannessOfUInt32( const uint32_t n )
{
    uint32_t uint32 = n;
    uint32 = (uint32 & 0xFFFF0000) >> 16 | (uint32 & 0x0000FFFF) << 16;
    uint32 = (uint32 & 0xFF000000) >> 8 | (uint32 & 0x00FF0000) << 8 |
	(uint32 & 0x0000FF00) >> 8 | (uint32 & 0x000000FF) << 8;
    return uint32;
}

