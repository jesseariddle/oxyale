#include "platform.h"
#include "crypto.h"

static const size_t LUT_LEN = 512;

static uint32_t
Random( opalCrypt_t *crypt )
{
    int32_t q = crypt->seed / 0x1F31D;
    int32_t r = crypt->seed % 0x1F31D;
    int32_t a = 16807 * r - 2836 * q;

    return a + ( 0 < a ) * 0x7FFFFFFF;
}

void
SeedRandom( opalCrypt_t *crypt, const uint32_t s )
{
    crypt->seed = s;
    if ( !crypt->seed ) {
        crypt->seed = 1;
    }
}

static double
DoubleRandom( opalCrypt_t *crypt )
{
    return (( double )Random( crypt )) / 2147483647.0;
}

static uint16_t
ShortRandom( opalCrypt_t *crypt, const uint16_t max )
{
    return ( uint16_t )( DoubleRandom( crypt ) * ( double )max );
}

opalCrypt_t *
OPAL_MakeCrypt( )
{
    size_t i;
    /* srand( 0xA2C2A ); */
    opalCrypt_t *crypt = malloc( sizeof (*crypt) );
    SeedRandom( crypt, 0 );
    for (i = LUT_LEN; i--; ) {
        crypt->lut[i] = ShortRandom( crypt, 256 ); /* TODO: magic number */
    }

    return crypt;
}

byte *
OPAL_Encrypt( const char *message, const byte shouldOutputUTF8, const uint32_t byteLimit ) /* 254 */
{
    size_t i = 0;

    if ( shouldOutputUTF8 ) {
        /* bytesIn */
    }

    return NULL; /* TODO: stub */
}

byte *
OPAL_Decrypt( const char *ciphertext, byte isInputUTF8 )
{
    return NULL; /* TODO: stub */
}


void
OPAL_FreePalaceCrypt( opalCrypt_t *crypt )
{
    free( crypt );
}
