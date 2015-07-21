//
//  palsay.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <palcom.h>
#include <palsay.h>

void OXLPalSayCmdDump(OXLPalSayCmd *sayCmd)
{
    fprintf(stderr, "sayCmd->cmdID = 0x%x\n", sayCmd->msgID);
    fprintf(stderr, "sayCmd->cmdLen = %d\n", sayCmd->msgLen);
    fprintf(stderr, "sayCmd->cmdRef = 0x%x\n", sayCmd->msgRef);
    fprintf(stderr, "sayCmd->len = %d\n", sayCmd->len);
    fprintf(stderr, "sayCmd->msg = %s\n", sayCmd->msg);
}

void OXLPalSayCmdInit(OXLPalSayCmd *sayCmd, OXLPalClient *client, char *text)
{
    /* oxl_say_t *say_msg = (oxl_say_t *)&buf.base; */
    char ct[PAL_CRYPTO_CIPHERTEXT_SZ_CAP];
    OXLPalCryptoEncrypt(client->crypto, text, ct);
    fprintf(stderr, "text: \"%s\", ct: \"%s\"\n", text, ct);
    uint z = (uint)strnlen(ct, PAL_CRYPTO_CIPHERTEXT_SZ_CAP);
    sayCmd->msgID = PAL_TX_SAY_CMD;
    sayCmd->msgLen = z + 3;
    sayCmd->msgRef = client->user.id;
    sayCmd->len = (ushort)z;
    memcpy(sayCmd->msg, ct, z);
}