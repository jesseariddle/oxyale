//
//  palsay.h
//  oxyale
//
//  Created by Jesse Riddle on 7/19/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palsay_h
#define oxyale_palsay_h

typedef struct OXLPalSayBubbleStruct {
    /* stub */
} OXLPalSayBubble;

typedef struct OXLPalSayCmdStruct {
    PAL_HEADER_FIELDS
    uint16_t len;
    char msg[PAL_CRYPT_CIPHERTEXT_SZ_CAP];
} OXLPalSayCmd;

void OXLPalInitSayCmd(OXLPalSayCmd *sayCmd, OXLPalClient *client, char *plainText);
void OXLPalDumpSayCmd(OXLPalSayCmd *sayCmd);

#endif
