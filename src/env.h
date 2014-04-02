#pragma once
#ifndef OPAL_ENV_H_

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

extern uint8_t OPAL_EnvIsBigEndian( );
extern uint32_t OPAL_UInt32SwapEndian( const uint32_t n );

#endif /* OPAL_ENV_H_ */
