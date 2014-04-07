#pragma once
#ifndef OX_REGCODE_H_
#define OX_REGCODE_H_

/* public interface */

typedef struct {
    uint32_t counter;
    uint32_t crc;
} ox_regcode_t;

void ox_regcode_from_str( ox_regcode_t *regcode, const char *str );
void ox_regcode_generate( ox_regcode_t *regcode );
void ox_regcode_print( const ox_regcode_t *regcode );
void ox_regcode_str_trim( char *trimmed_regcode_str, const char *str );
int32_t ox_regcode_str_trim_len( const char *str );

#endif /* OPAL_REGCODE_H_ */
