//
//  palsay.h
//  oxyale
//
//  Created by Jesse Riddle on 7/19/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palsay_h
#define oxyale_palsay_h

#include <palclient.h>

typedef struct OXLPalSayCmdStruct {
    PAL_HEADER_FIELDS
    uint16_t len;
    char msg[PAL_CRYPTO_CIPHERTEXT_SZ_CAP];
} OXLPalSayCmd;

void OXLPalSayCmdInit(OXLPalSayCmd *sayCmd, OXLPalClient *client, char *plainText);
void OXLPalSayCmdDump(OXLPalSayCmd *sayCmd);

#endif
