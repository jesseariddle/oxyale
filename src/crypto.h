#pragma once
#ifndef _OPAL_CYRPT_H_

#include "platform.h"

#define OPAL_CRYPT_LUT_LEN 512
#define OPAL_CRYPT_LUT_MAX 256

typedef struct {
    uint32_t seed;
    byte lut[OPAL_CRYPT_LUT_LEN];
} opalCrypt_t;

extern opalCrypt_t *OPAL_MakeCrypt( );
extern byte *OPAL_Encrypt( const char *message, byte shouldOutputUTF8, uint32_t byteLimit );
extern byte *OPAL_Decrypt( const char *ciphertext, byte isInputUTF8 );
extern void OPAL_FreePalaceCrypt( opalCrypt_t *crypt );

#endif /* _OPAL_CRYPT_H_ */
