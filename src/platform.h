#pragma once
#ifndef _OPAL_PLATFORM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#ifndef YES
#define YES 1
#endif
#ifndef NO
#define NO 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NIL
#define NIL 0
#endif
#ifndef NULL
#define NULL 0
#endif

#ifndef __cplusplus
#define false 0
#define true 1
#endif

typedef unsigned char byte;

extern uint8_t OPAL_PlatformIsBigEndian( );
extern uint32_t OPAL_SwapEndiannessOfUInt32( const uint32_t n );

#endif /* _OPAL_PLATFORM_H_ */
