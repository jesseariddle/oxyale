#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../../include/env.h"
#include "../../include/ox_crypt.h"
#include "../../include/ox_regcode.h"
#include "../../include/ox_msg.h"

void crypto_test()
{
    struct timeval stop, start;

    printf( "Crypto test -----\n" );
    gettimeofday( &start, NULL );

    char original_text[] = "Hello, world!";

    /* ox_crypt_t *ox_crypt = malloc( sizeof (*ox_crypt) ); */
    ox_crypt_t *ox_crypt = alloca( sizeof (*ox_crypt) );
    ox_crypt_init( ox_crypt );

    /* char *ciphertext = calloc( strnlen( original_text, 254 ) + 1, sizeof (*ciphertext) ); */
    char *ciphertext = alloca( strnlen( original_text, CHAR_BIT * sizeof (uint8_t) ) );
    ox_encrypt( ciphertext, ox_crypt, original_text );

    char *plaintext = alloca( strnlen( ciphertext, CHAR_BIT * sizeof (uint8_t) ) );
    ox_decrypt( plaintext, ox_crypt, ciphertext );

    /* free( ciphertext ); */
    /* free( plain_text ); */
    /* free( ox_crypt ); */

    gettimeofday( &stop, NULL );
    printf("original_text: %s\n", original_text);
    printf("ciphertext: %s\n", ciphertext);
    printf("plaintext: %s\n", plaintext);
    printf( "time spent: %d µs\n\n", stop.tv_usec - start.tv_usec );
}

void regcode_test()
{
    struct timeval stop, start;
    printf( "Regcode test -----\n" );
    char *test_regcode_str = "9YAT-C8MM-GJVZL";
    gettimeofday( &start, NULL );

    /* ox_regcode_t *regcode = malloc( sizeof (*regcode) ); */
    ox_regcode_t *regcode = alloca( sizeof (*regcode) );
    ox_regcode_generate( regcode );

    /* char *trimmed_regcode_str = calloc( ox_regcode_str_trim_len( test_regcode_str ) + 1, sizeof (*test_regcode_trimmed_str) ); */
    char *trimmed_regcode_str = alloca( ox_regcode_str_trim_len( test_regcode_str ) + 1 );
    ox_regcode_str_trim( trimmed_regcode_str, test_regcode_str );

    ox_regcode_t *another_regcode = alloca( sizeof (*regcode) );
    ox_regcode_from_str( another_regcode, trimmed_regcode_str );

    /* free( regcode ); */
    /* free( another_regcode ); */
    /* free( test_regcode_trimmed_str ); */

    gettimeofday( &stop, NULL );
    ox_regcode_print( regcode );
    ox_regcode_print( another_regcode );
    printf( "time spent: %d µs\n\n", stop.tv_usec - start.tv_usec );
}

void room_msg_test()
{
    /* not ready yet, don't call this method */
    /* byte *data; */
    /* int32_t ref_id = 0; */
    /* ox_room_msg_t *room_msg = malloc( ox_room_msg_len( data ) ); */
    /* ox_room_msg_init( room_msg, data, ref_id ); */
}

int main()
{
    regcode_test();
    crypto_test();
    /* room_test(); */

    return 0;
}
