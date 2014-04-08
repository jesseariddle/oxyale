#pragma once
#ifndef OX_CYRPT_H_
#define OX_CRYPT_H_

#define OX_CRYPT_LUT_LEN 512
#define OX_CRYPT_LUT_MAX 256
#define OX_PLAINTEXT_MAX 255
#define OX_CIPHERTEXT_MAX 255

typedef struct {
    int32_t seed;
    unsigned char lut[OX_CRYPT_LUT_LEN];
} ox_crypt_t;

void ox_crypt_init( ox_crypt_t *ox_crypt );
void ox_encrypt( char *encrypted_buf, const ox_crypt_t *ox_crypt, const char *plain_text );
void ox_decrypt( char *decrypted_buf, const ox_crypt_t *ox_crypt, const char *cipher_text );

#endif /* OX_CRYPT_H_ */
