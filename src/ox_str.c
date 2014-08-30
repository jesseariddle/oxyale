#include "env.h"
#include "ox_str.h"

static uint8_t ox_str_should_escape( char *c )
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

/* static uint32_t ox_str_escape_with_char( char *c ) */
/* { */
/*     if ( *c & 0x80 ) */
/*         return 0;    /\* ignore multibyte chars *\/ */
/*     else if ( *c < 0x1F || *c == 0x7F ) */
/*         return *c;   /\* control chars must be escaped *\/ */
/*     int i = *c >> 3; */
/*     byte mask = ( ( byte * )c )[i]; */
/*     if ( mask & ( 1 << ( *c & 0x7 ) ) ) */
/*         return *c; */
/*     else */
/*         return 0; */
/* } */

int32_t ox_str_uri_escape_len( char *unescaped_str )
{
    int32_t c, i, y = 0, z = strnlen( unescaped_str, OX_STR_UINT16_MAX );
    for ( i = z; i--; ) {
        c = ox_str_should_escape( &unescaped_str[i] );
        y += ( c << 1 ) + c;
    }
    return y;
}

void ox_str_uri_escape( char *escaped_str, char *unescaped_str )
{
    int32_t c = 0, i, x, z = strnlen( unescaped_str, OX_STR_UINT16_MAX );
    for ( i = 0; i < z; ++i ) {
        x = ox_str_should_escape( &unescaped_str[i] );
        if ( x ) {
            snprintf( escaped_str + i, 3, "%%%02x", x );
            c += 3;
        } else
            escaped_str[c++] = unescaped_str[i];
    } escaped_str[c] = '\0';
}
