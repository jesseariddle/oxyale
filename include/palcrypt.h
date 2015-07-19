//
//  oxyale-palcrypt.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_oxyale_palcrypt_h
#define oxyale_oxyale_palcrypt_h

#define PAL_CRYPT_LUT_LEN           512
#define PAL_CRYPT_LUT_MAX           256
#define PAL_CRYPT_PLAINTEXT_SZ_CAP  255
#define PAL_CRYPT_CIPHERTEXT_SZ_CAP 255

typedef struct OXLPalCryptStruct {
    int32_t seed;
    int8_t lut[PAL_CRYPTO_LUT_LEN];
} OXLPalCrypt;

void OXLPalInitCrypt(OXLPalCrypt *crypt);
void OXLPalEncrypt(const OXLPalCrypt crypt, const char *plainText, char *cipherText);
void OXLPalDecrypt(const OXLPalCrypt crypt, const char *cipherText, char *plainText);

#endif
