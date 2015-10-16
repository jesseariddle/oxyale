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

typedef struct OXLPalSayMsgStruct {
    const OXLPalMsg palMsg;
    const uint16_t len;
    const char msg[PAL_CRYPTO_CIPHERTEXT_SZ_CAP];
} OXLPalSayMsg;

void OXLInitPalSayMsg(OXLPalSayMsg *sayMsg, OXLPalClient *client, const char *plainText);
OXLPalSayMsg *OXLCreatePalSayMsg(OXLPalClient *client, const char *plainText);
void OXLDumpPalSayMsg(const OXLPalSayMsg *sayMsg);

#endif
