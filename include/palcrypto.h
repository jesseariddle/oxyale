//
//  oxyale-palcrypto.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palcrypto_h
#define oxyale_palcrypto_h

#include "palcom.h"

typedef struct OXLPalCryptoStruct {
    int32_t seed;
    int8_t lut[PAL_CRYPTO_LUT_LEN];
} OXLPalCrypto;

void OXLInitPalCrypto(OXLPalCrypto *crypto);
OXLPalCrypto *OXLMakePalCrypto(void);
void OXLReleasePalCrypto(OXLPalCrypto *crypto);
void OXLPalCryptoEncrypt(const OXLPalCrypto crypto, const char *plainText, char *cipherText);
void OXLPalCryptoDecrypt(const OXLPalCrypto crypto, const char *cipherText, char *plainText);

#endif
