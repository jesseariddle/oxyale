#include <oxyale.h>

static inline void seed_random(oxl_crypt_t *ox_crypt, const int32_t s)
{
    ox_crypt->seed = s | !s;
}

static inline int32_t int32_random(oxl_crypt_t *self)
{
    int32_t q = self->seed / 0x1f31d;
    int32_t r = self->seed % 0x1f31d;
    int32_t a = 16807 * r - 2836 * q;
    /* if 0 < a: a, else: a + 0x7fffffff */
    self->seed = a + (a <= 0) * 0x7fffffff;
    return self->seed;
}

static inline double double_random(oxl_crypt_t *self)
{
    return ((double)int32_random(self)) / 2147483647.0;
}

static inline int16_t int16_random(oxl_crypt_t *self, const int16_t max)
{
    return (int16_t)(double_random(self) * (double)max);
}

void oxl_crypt_init(oxl_crypt_t *self)
{
    size_t i;
    seed_random(self, 0xa2c2a);
    for (i = OXL_CRYPT_LUT_LEN; i--;) {
        self->lut[i] = int16_random(self, OXL_CRYPT_LUT_MAX);
    }
}

void oxl_crypt_encrypt(const oxl_crypt_t crypt,
                       const uv_buf_t pt,
                       uv_buf_t *ct);
{
    /* OXL_CRYPT_PLAINTEXT_SZ_CAP */
    size_t c = 0, i; /* , z = strnlen(pt, OXL_CRYPT_PLAINTEXT_SZ_LEN); */
    char prev_char = '\0';
    for (i = ct.len - 1; i--;) {
        ct[i] = pt[i] ^ crypt->lut[c++] ^ prev_char;
        prev_char = ct[i] ^ crypt->lut[c++];
    } ct[z] = '\0';
}

void oxl_crypt_decrypt(const oxl_crypt_t crypt,
                       const uv_buf_t ct,
                       uv_buf_t *pt)
{
    size_t c = 0, i; /* z = strnlen(ct, OXL_CRYPT_CIPHERTEXT_SZ_LEN); */
    char prev_char = '\0';
    for (i = ct.len - 1; i--;) {
        pt[i] = ct[i] ^ crypt->lut[c++] ^ prev_char;
        prev_char = ct[i] ^ crypt->lut[c++];
    } pt[z] = '\0';
}
