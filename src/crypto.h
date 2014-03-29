#pragma once
#ifndef _OPAL_CYRPTO_H_

#include "platform.h"

typedef struct {
    uint32_t seed;
    byte lut[512]; /* TODO: magic number */
} opalCrypt_t;

extern opalCrypt_t *OPAL_MakeCrypt( );
extern byte *OPAL_Encrypt( const char *message, byte shouldOutputUTF8, uint32_t byteLimit );
extern byte *OPAL_Decrypt( const char *ciphertext, byte isInputUTF8 );
extern void OPAL_FreePalaceCrypt( opalCrypt_t *crypt );

#endif /* _OPAL_CRYPTO_H_ */
