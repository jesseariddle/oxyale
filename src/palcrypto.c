#include <palcom.h>
#include <palcrypto.h>

static inline void OXLSeedRandom(OXLPalCrypto *crypto, const int32_t s)
{
    crypto->seed = s | !s;
}

static inline int32_t OXLRandomInt32(OXLPalCrypto *crypto)
{
    int32_t q = crypto->seed / 0x1f31d;
    int32_t r = crypto->seed % 0x1f31d;
    int32_t a = 16807 * r - 2836 * q;
    /* if 0 < a: a, else: a + 0x7fffffff */
    crypto->seed = a + (a <= 0) * 0x7fffffff;
    return crypto->seed;
}

static inline double OXLRandomDouble(OXLPalCrypto *crypto)
{
    return ((double)OXLRandomInt32(crypto)) / 2147483647.0;
}

static inline int16_t OXLRandomInt16(OXLPalCrypto *crypto, const int16_t max)
{
    return (int16_t)(OXLRandomDouble(crypto) * (double)max);
}

void OXLInitPalCrypto(OXLPalCrypto *crypto)
{
    size_t i;
    OXLSeedRandom(crypto, 0xa2c2a);
    for (i = PAL_CRYPTO_LUT_LEN; i--;) {
        crypto->lut[i] = OXLRandomInt16(crypto, PAL_CRYPTO_LUT_MAX);
    }
}

void OXLPalEncrypto(const OXLPalCrypto crypto, const char *plainText, char *cipherText)
{
    size_t c = 0, i, len = strnlen(plainText, PAL_CRYPTO_PLAINTEXT_SZ_CAP);
    char prev_char = '\0';
    for (i = len - 1; i--;) {
        cipherText[i] = plainText[i] ^ crypto.lut[c++] ^ prev_char;
        prev_char = cipherText[i] ^ crypto.lut[c++];
    } cipherText[len] = '\0';
}

void OXLPalDecrypto(const OXLPalCrypto crypto, const char *cipherText, char *plainText)
{
    size_t c = 0, i, len = strnlen(cipherText, PAL_CRYPTO_CIPHERTEXT_SZ_CAP);
    char prev_char = '\0';
    for (i = len - 1; i--;) {
        plainText[i] = cipherText[i] ^ crypto.lut[c++] ^ prev_char;
        prev_char = cipherText[i] ^ crypto.lut[c++];
    } plainText[len] = '\0';
}
