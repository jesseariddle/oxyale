#include <oxyale.h>
#include <oxl-pal-crypt.h>

static inline void OXLSeedRandom(OXLPalCrypt *crypt, const int32_t s)
{
    crypt->seed = s | !s;
}

static inline int32_t OXLRandomInt32(OXLPalCrypt *crypt)
{
    int32_t q = crypt->seed / 0x1f31d;
    int32_t r = crypt->seed % 0x1f31d;
    int32_t a = 16807 * r - 2836 * q;
    /* if 0 < a: a, else: a + 0x7fffffff */
    crypt->seed = a + (a <= 0) * 0x7fffffff;
    return crypt->seed;
}

static inline double OXLRandomDouble(OXLPalCrypt *crypt)
{
    return ((double)OXLRandomInt32(crypt)) / 2147483647.0;
}

static inline int16_t OXLRandomInt16(OXLPalCrypt *crypt, const int16_t max)
{
    return (int16_t)(OXLRandomDouble(crypt) * (double)max);
}

void OXLInitPalCrypt(OXLPalCrypt *crypt)
{
    size_t i;
    OXLSeedRandom(crypt, 0xa2c2a);
    for (i = PAL_CRYPT_LUT_LEN; i--;) {
        crypt->lut[i] = OXLRandomInt16(crypt, PAL_CRYPT_LUT_MAX);
    }
}

void OXLPalEncrypt(const OXLPalCrypt crypt, const char *plainText, char *cipherText)
{
    size_t c = 0, i, len = strnlen(plainText, PAL_CRYPT_PLAINTEXT_SZ_CAP);
    char prev_char = '\0';
    for (i = len - 1; i--;) {
        cipherText[i] = plainText[i] ^ crypt.lut[c++] ^ prev_char;
        prev_char = cipherText[i] ^ crypt.lut[c++];
    } cipherText[len] = '\0';
}

void OXLPalDecrypt(const OXLPalCrypt crypt, const char *cipherText, char *plainText)
{
    size_t c = 0, i, len = strnlen(cipherText, PAL_CRYPT_CIPHERTEXT_SZ_CAP);
    char prev_char = '\0';
    for (i = len - 1; i--;) {
        plainText[i] = cipherText[i] ^ crypt.lut[c++] ^ prev_char;
        prev_char = cipherText[i] ^ crypt.lut[c++];
    } plainText[len] = '\0';
}
