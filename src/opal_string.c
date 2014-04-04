#include "env.h"
#include "opal_string.h"

static uint8_t
URIShouldEscape( char *c )
{
    if ( *c & 0x80 )
        return 0;    /* ignore multibyte chars */
    else if ( *c < 0x1F || *c == 0x7F )
        return 1;    /* control chars must be escaped */

    int i = *c >> 3;
    byte mask = ( ( byte * )c )[i];
    if ( mask & ( 1 << ( *c & 0x7 ) ) )
        return 1;
    else
        return 0;
}

static uint32_t
URIShouldEscapeWithChar( char *c )
{
    if ( *c & 0x80 )
        return 0;    /* ignore multibyte chars */
    else if ( *c < 0x1F || *c == 0x7F )
        return *c;   /* control chars must be escaped */

    int i = *c >> 3;
    byte mask = ( ( byte * )c )[i];
    if ( mask & ( 1 << ( *c & 0x7 ) ) )
        return *c;
    else
        return 0;
}

char *
OPAL_MakeEscapedURIString( char *unescaped_s )
{
    int32_t c, i, x, y = 0, z = strlen( unescaped_s );

    for ( i = z; i--; ) {
        c = URIShouldEscape( &unescaped_s[i] );        
        y += ( c << 1 ) + c;
    }

    char *escaped_s = calloc( y + 1, sizeof (*escaped_s) );
    for ( i = 0; i < z; ++i ) {
        x = URIShouldEscapeWithChar( &unescaped_s[i] );
        if ( x )
            snprintf( escaped_s + i, 3, "%%%02x", x );
    }

    return escaped_s;
}

void
OPAL_FreeEscapedURIString( char *escaped_s )
{
    free( escaped_s );
}
