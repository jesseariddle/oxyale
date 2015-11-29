//
//  palcom.c
//  oxyale
//
//  Created by Jesse Riddle on 10/12/15.
//  Copyright © 2015 Riddle Software. All rights reserved.
//

#include <palcom.h>

extern OXLPalMsg const kPalPingMsg;

void
OXLResetPalMsg(OXLPalMsg *palMsg)
{
    palMsg->id = 0;
    palMsg->len = 0;
    palMsg->ref = 0;
}

void
OXLInitPalMsg(OXLPalMsg *palMsg, uint32_t id, uint32_t len, uint32_t ref)
{
    palMsg->id = id;
    palMsg->len = len;
    palMsg->ref = ref;
}

void
OXLDumpPalMsg(const OXLPalMsg *palMsg)
{
    char msgText[5] = { '\0' };
    memcpy(&msgText, &palMsg->id, 4);
    msgText[4] = '\0';
    OXLLogDebug("palMsg->id = 0x%x (%s)", palMsg->id, msgText);
    OXLLogDebug("palMsg->len = 0x%x (%u)", palMsg->len, palMsg->len);
    OXLLogDebug("palMsg->ref = 0x%x", palMsg->ref);
}

OXLPalMsg *
OXLCreatePalMsg(uint32_t id, uint32_t ref)
{
    OXLPalMsg *palMsg = OXLAlloc(sizeof(*palMsg));
    palMsg->id = id;
    palMsg->len = 0;
    palMsg->ref = ref;
    
    return palMsg;
}