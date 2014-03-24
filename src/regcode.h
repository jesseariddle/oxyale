#pragma once
#ifndef _OPAL_REGCODE_H_
#define _OPAL_REGCODE_H_

/* public interface */

typedef struct OPAL_Regcode {
    unsigned int counter;
    unsigned int crc;
} OPAL_Regcode;

extern OPAL_Regcode *OPAL_MakeRegcode();
extern OPAL_Regcode *OPAL_MakeRegcodeFromString(char *RegcodeString, int Length);
extern void OPAL_PrintRegcode();
extern void OPAL_FreeRegcode(OPAL_Regcode *Regcode);

#endif /* _OPAL_REGCODE_H_ */
