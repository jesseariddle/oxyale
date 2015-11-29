#include <oxlcom.h>
#include <palcrypto.h>

void
OXLPalCryptoSeedRandom(OXLPalCrypto *crypto, const int32_t s)
{
    /* if s == 0: s = 1 else: s */
    /* crypto->seed = s | !s; */
    crypto->seed = s + !s;
}

int32_t
OXLPalCryptoRandomInt32(OXLPalCrypto *crypto)
{
    int32_t q = crypto->seed / 0x1f31d;
    int32_t r = crypto->seed % 0x1f31d;
    int32_t a = 16807 * r - 2836 * q;
    /* if 0 < a: a, else: a + 0x7fffffff */
    crypto->seed = a + (a <= 0) * 0x7fffffff;
    return crypto->seed;
}

double
OXLPalCryptoRandomDouble(OXLPalCrypto *crypto)
{
    return ((double)OXLPalCryptoRandomInt32(crypto)) / 2147483647.0; /* 0x7fffffff */
}

int16_t
OXLPalCryptoRandomInt16(OXLPalCrypto *crypto, const int16_t max)
{
    return (int16_t)(OXLPalCryptoRandomDouble(crypto) * (double)max);
}

void
OXLInitPalCrypto(OXLPalCrypto *crypto)
{
    size_t i;
    OXLPalCryptoSeedRandom(crypto, 0xa2c2a);
    for (i = PAL_CRYPTO_LUT_LEN; i--;) {
        crypto->lut[i] = OXLPalCryptoRandomInt16(crypto, PAL_CRYPTO_LUT_MAX);
    }
    OXLPalCryptoSeedRandom(crypto, 1);
}

void
OXLPalCryptoEncrypt(const OXLPalCrypto crypto, const char *plainText, char *cipherText)
{
    size_t c = 0, i, len = strnlen(plainText, PAL_CRYPTO_PLAINTEXT_SZ_CAP);
    char prevChar = '\0';
    for (i = len - 1; i--;) {
        cipherText[i] = plainText[i] ^ crypto.lut[c++] ^ prevChar;
        prevChar = cipherText[i] ^ crypto.lut[c++];
    } cipherText[len] = '\0';
}

void
OXLPalCryptoDecrypt(const OXLPalCrypto crypto, const char *cipherText, char *plainText)
{
    size_t c = 0, i, len = strnlen(cipherText, PAL_CRYPTO_CIPHERTEXT_SZ_CAP);
    char prevChar = '\0';
    for (i = len - 1; i--;) {
        plainText[i] = cipherText[i] ^ crypto.lut[c++] ^ prevChar;
        prevChar = cipherText[i] ^ crypto.lut[c++];
    } plainText[len] = '\0';
}

OXLPalCrypto *
OXLMakePalCrypto(void)
{
    OXLPalCrypto *crypto = malloc(sizeof(*crypto));
    return crypto;
}

void
OXLReleasePalCrypto(OXLPalCrypto *crypto)
{
    free(crypto);
}
