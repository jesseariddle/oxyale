#pragma once
#ifndef OX_STR_H_
#define OX_STR_H_

#define OX_STR_UINT16_MAX 65534
#define OX_STR_UINT8_MAX 254

typedef struct {
    uint8_t len;
    char *text;
} ox_uint8_string_t;

typedef struct {
    uint16_t len;
    char *text;
} ox_uint16_string_t;

void ox_str_uri_escape( char *escaped_str, char *unescaped_str );

#endif /* OX_STR_H_ */
