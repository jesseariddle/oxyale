//
//  pallogon.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <palping.h>

OXLPalPingMsg const gPalPingMsg = { .palMsg.id = PAL_TX_PING_MSG, .palMsg.len = 0, .palMsg.ref = 0 };

void OXLInitPalPingMsg(OXLPalPingMsg *pingMsg)
{
    OXLLogDebug("OXLInitPalPingMsg");
    
    OXLPalPingMsg palPingMsgInit = { .palMsg.id = PAL_TX_PING_MSG, .palMsg.len = 0, .palMsg.ref = 0 };
    memcpy(pingMsg, &palPingMsgInit, sizeof(*pingMsg));
}

OXLPalPingMsg *OXLCreatePalPingMsg(void)
{
    OXLLogDebug("OXLCreatePalPingMsg");
    OXLPalPingMsg *pingMsg = OXLAlloc(sizeof(*pingMsg));
    OXLInitPalPingMsg(pingMsg);
    return pingMsg;
}

void OXLDumpPalPingMsg(const OXLPalPingMsg *pingMsg)
{
    OXLLogDebug("OXLDumpPalPingMsg");
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)logon_buf->base; */
    /* raw */
    OXLLogDebug("pingMsg->palMsg.id = 0x%x", pingMsg->palMsg.id);
    OXLLogDebug("pingMsg->palMsg.len = %d", pingMsg->palMsg.len);
    OXLLogDebug("pingMsg->palMsg.ref = 0x%x", pingMsg->palMsg.ref);
}

void OXLDestroyPalPingMsg(OXLPalPingMsg *pingMsg)
{
    OXLFree(pingMsg);
}