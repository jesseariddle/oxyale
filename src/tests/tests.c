#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../src/env.h"
#include "../../src/opal_crypt.h"
#include "../../src/opal_regcode.h"

void crypto_test( )
{
    struct timeval stop, start;

    printf( "Crypto test -----\n" );
    gettimeofday( &start, NULL );
    opalCrypt_t *crypt = OPAL_MakeCrypt( );
    char originaltext[] = "Hello, world!";
    printf("originaltext: %s\n", originaltext);

    char *ciphertext = OPAL_MakeEncryptedMessage( crypt, originaltext, 254 );
    printf("ciphertext: %s\n", ciphertext);

    char *plaintext = OPAL_MakeDecryptedMessage( crypt, ciphertext );
    printf("plaintext: %s\n", plaintext);

    OPAL_FreeMessage( ciphertext);
    OPAL_FreeMessage( plaintext );
    OPAL_FreeCrypt( crypt );

    gettimeofday( &stop, NULL );
    printf( "time spent: %d µs\n\n", stop.tv_usec - start.tv_usec );
}

void regcode_test( )
{
    struct timeval stop, start;
    printf( "Regcode test -----\n" );
    gettimeofday( &start, NULL );
    opalRegcode_t *regcode = OPAL_MakeRegcode( );
    opalRegcode_t *anotherRegcode = OPAL_MakeRegcodeFromString( "9YAT-C8MM-GJVZL" );

    OPAL_PrintRegcode( regcode );
    OPAL_PrintRegcode( anotherRegcode );

    OPAL_FreeRegcode( regcode );
    OPAL_FreeRegcode( anotherRegcode );

    gettimeofday( &stop, NULL );
    printf( "time spent: %d µs\n\n", stop.tv_usec - start.tv_usec );
}

int main( )
{
    regcode_test();
    crypto_test();

    return 0;
}
