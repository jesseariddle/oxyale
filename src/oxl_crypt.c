#include <oxyale.h>

static inline void seedRandom(PalCrypt *crypt, const int32_t s)
{
    crypt->seed = s | !s;
}

static inline int32_t randomInt32(PalCrypt *crypt)
{
    int32_t q = crypt->seed / 0x1f31d;
    int32_t r = crypt->seed % 0x1f31d;
    int32_t a = 16807 * r - 2836 * q;
    /* if 0 < a: a, else: a + 0x7fffffff */
    crypt->seed = a + (a <= 0) * 0x7fffffff;
    return crypt->seed;
}

static inline double randomDouble(PalCrypt *crypt)
{
    return ((double)randomInt32(crypt)) / 2147483647.0;
}

static inline int16_t randomInt16(PalCrypt *crypt, const int16_t max)
{
    return (int16_t)(randomDouble(crypt) * (double)max);
}

void InitPalCrypt(PalCrypt *crypt)
{
    size_t i;
    seedRandom(crypt, 0xa2c2a);
    for (i = PAL_CRYPT_LUT_LEN; i--;) {
        self->lut[i] = randomInt16(crypt, PAL_CRYPT_LUT_MAX);
    }
}

void PalEncrypt(const PalCrypt crypt, const uv_buf_t pt, uv_buf_t *ct);
{
    /* OXL_CRYPT_PLAINTEXT_SZ_CAP */
    size_t c = 0, i; /* , z = strnlen(pt, PAL_CRYPT_PLAINTEXT_SZ_LEN); */
    char prev_char = '\0';
    for (i = ct.len - 1; i--;) {
        ct[i] = pt[i] ^ crypt->lut[c++] ^ prev_char;
        prev_char = ct[i] ^ crypt->lut[c++];
    } ct[z] = '\0';
}

void PalDecrypt(const PalCrypt crypt, const uv_buf_t ct, uv_buf_t *pt)
{
    size_t c = 0, i; /* z = strnlen(ct, PAL_CRYPT_CIPHERTEXT_SZ_LEN); */
    char prev_char = '\0';
    for (i = ct.len - 1; i--;) {
        pt[i] = ct[i] ^ crypt->lut[c++] ^ prev_char;
        prev_char = ct[i] ^ crypt->lut[c++];
    } pt[z] = '\0';
}
