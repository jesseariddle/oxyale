#pragma once
#ifndef _OPAL_CYRPT_H_

#define OPAL_CRYPT_LUT_LEN 512
#define OPAL_CRYPT_LUT_MAX 256

typedef struct {
    int32_t seed;
    unsigned char lut[OPAL_CRYPT_LUT_LEN];
} opalCrypt_t;

extern opalCrypt_t *OPAL_MakeCrypt( );
extern char *OPAL_MakeEncryptedMessage( const opalCrypt_t *crypt, const char *message, uint32_t byteLimit );
extern char *OPAL_MakeDecryptedMessage( const opalCrypt_t *crypt, const char *ciphertext );
extern void OPAL_FreeMessage( char *message );
extern void OPAL_FreeCrypt( opalCrypt_t *crypt );

#endif /* _OPAL_CRYPT_H_ */
