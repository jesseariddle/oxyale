//
//  oxyale-palreg.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palreg_h
#define oxyale_palreg_h

#define PAL_REG_TO_ASCII_LUT_LEN 32
#define PAL_REG_STR_LEN          13

#define PAL_REG_LOWER_CODE_TO_ASCII_LUT "abcdefghjklmnpqrstuvwxyz23456789"
#define PAL_REG_UPPER_CODE_TO_ASCII_LUT "ABCDEFGHJKLMNPQRSTUVWXYZ23456789"

#define PAL_REG_CRC_MAGIC  0xA95ADE76
#define PAL_REG_MAGIC_LONG 0x9602C9BF

extern uint32_t PAL_REG_CRC_MASK[];

typedef struct OXLPalRegStruct {
    uint32_t counter;
    uint32_t crc;
} OXLPalReg;

void OXLPalStringToReg(const char *str, OXLPalReg *reg);
void OXLPalGenerateReg(OXLPalReg *reg);
void OXLPalPrintReg(const OXLPalReg *reg);
void OXLPalTrimRegString(char *trimmedRegString, const char *str);
int32_t OXLPalTrimLenOfRegString(const char *str);

#endif
