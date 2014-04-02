#pragma once
#ifndef OPAL_REGCODE_H_
#define OPAL_REGCODE_H_

/* public interface */

typedef struct {
    uint32_t counter;
    uint32_t crc;
} opalRegcode_t;

extern opalRegcode_t *OPAL_MakeRegcode( void );
extern opalRegcode_t *OPAL_MakeRegcodeFromString( const char *s );
extern void OPAL_PrintRegcode( const opalRegcode_t *regcode );
extern void OPAL_FreeRegcode( opalRegcode_t *regcode );

#endif /* OPAL_REGCODE_H_ */
