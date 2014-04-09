#include "env.h"
#include "ox_crypt.h"

static inline void seed_random( ox_crypt_t *ox_crypt, const int32_t s )
{
    ox_crypt->seed = s | !s;
}

static inline int32_t int32_random( ox_crypt_t *ox_crypt )
{
    int32_t q = ox_crypt->seed / 0x1f31d;
    int32_t r = ox_crypt->seed % 0x1f31d;
    int32_t a = 16807 * r - 2836 * q;
    /* if 0 < a: a, else: a + 0x7fffffff */
    ox_crypt->seed = a + ( a <= 0 ) * 0x7fffffff;
    return ox_crypt->seed;
}

static inline double double_random( ox_crypt_t *ox_crypt )
{
    return ( ( double )int32_random( ox_crypt ) ) / 2147483647.0;
}

static inline int16_t int16_random( ox_crypt_t *ox_crypt, const int16_t max )
{
    return ( int16_t )( double_random( ox_crypt ) * ( double )max );
}

void ox_crypt_init( ox_crypt_t *ox_crypt )
{
    size_t i;
    seed_random( ox_crypt, 0xa2c2a );
    for ( i = OX_CRYPT_LUT_LEN; i--; )
        ox_crypt->lut[i] = int16_random( ox_crypt, OX_CRYPT_LUT_MAX );
}

void ox_encrypt( char *encrypted_buf, const ox_crypt_t *ox_crypt, const char *plain_text )
{
    size_t c = 0, i, z = strnlen( plain_text, OX_PLAINTEXT_MAX );
    char last_char = '\0';

    for ( i = z; i--; ) {
        encrypted_buf[i] = ( char )( plain_text[i] ^ ox_crypt->lut[c++] ^ last_char );
        last_char = ( char )( encrypted_buf[i] ^ ox_crypt->lut[c++] );
    } encrypted_buf[z] = '\0';
}

void ox_decrypt( char *decrypted_buf, const ox_crypt_t *ox_crypt, const char *cipher_text )
{
    size_t c = 0, i, z = strnlen( cipher_text, OX_CIPHERTEXT_MAX );
    char last_char = '\0';

    for ( i = z; i--; ) {
        decrypted_buf[i] = ( char )( cipher_text[i] ^ ox_crypt->lut[c++] ^ last_char );
        last_char = ( char )( cipher_text[i] ^ ox_crypt->lut[c++] );
    } decrypted_buf[z] = '\0';
}
