//
//  palsay.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <oxlcom.h>
#include <palsay.h>

void
OXLDumpPalSayMsg(const OXLPalSayMsg *sayMsg)
{
    OXLLogDebug("sayMsg->palMsg.id = 0x%x\n", sayMsg->palMsg.id);
    OXLLogDebug("sayMsg->palMsg.len = %d\n", sayMsg->palMsg.len);
    OXLLogDebug("sayMsg->palMsg.ref = 0x%x\n", sayMsg->palMsg.ref);
    OXLLogDebug("sayMsg->len = %d\n", sayMsg->len);
    OXLLogDebug("sayMsg->msg = %s\n", sayMsg->msg);
}

void
OXLInitPalSayMsg(OXLPalSayMsg *sayMsg, OXLPalClient *client, const char *plainText)
{
    /* oxl_say_t *say_msg = (oxl_say_t *)&buf.base; */
    char cipherText[PAL_CRYPTO_CIPHERTEXT_SZ_CAP];
    OXLPalCryptoEncrypt(client->crypto, plainText, cipherText);
    OXLLogDebug("plainText: \"%s\"\ncipherText: \"%s\"\n", plainText, cipherText);
    uint16_t z = (uint16_t)strnlen(cipherText, PAL_CRYPTO_CIPHERTEXT_SZ_CAP);
    
    OXLPalSayMsg initPalSayMsg = { .palMsg.id = PAL_TX_SAY_MSG,
                                   .palMsg.len = z + 3,
                                   .palMsg.ref = client->user.id,
                                   .len = z };
    
    memcpy((OXLPalSayMsg *)sayMsg, &initPalSayMsg, sizeof(*sayMsg));
    /* memcpy((char *)&sayMsg->msg, cipherText, z); */
    strlcpy((char *)&sayMsg->msg, cipherText, PAL_CRYPTO_CIPHERTEXT_SZ_CAP);
}

OXLPalSayMsg *
OXLCreatePalSayMsg(OXLPalClient *client, const char *plainText)
{
    OXLPalSayMsg *sayMsg = malloc(sizeof(*sayMsg));
    OXLInitPalSayMsg(sayMsg, client, plainText);
    return sayMsg;
}
