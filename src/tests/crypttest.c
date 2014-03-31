#include <stdio.h>
#include <stdlib.h>
#include "../../src/crypt.h"

int main( )
{
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
    return 0;
}
