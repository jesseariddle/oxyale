#pragma once
#ifndef OX_ENV_H_
#define OX_ENV_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <limits.h>

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

extern uint8_t ox_env_is_big_endian();
extern uint32_t ox_uint32_swap_endian(const uint32_t n);
extern void ox_strify(char *str, const uint32_t uint32);

#endif /* OX_ENV_H_ */
