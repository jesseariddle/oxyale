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
    OXLLog("OXLInitPalPingMsg");
    
    OXLPalPingMsg palPingMsgInit = { .palMsg.id = PAL_TX_PING_MSG, .palMsg.len = 0, .palMsg.ref = 0 };
    memcpy(pingMsg, &palPingMsgInit, sizeof(*pingMsg));
}

OXLPalPingMsg *OXLCreatePalPingMsg(void)
{
    OXLLog("OXLCreatePalPingMsg");
    OXLPalPingMsg *pingMsg = OXLAlloc(sizeof(*pingMsg));
    OXLInitPalPingMsg(pingMsg);
    return pingMsg;
}

void OXLDumpPalPingMsg(const OXLPalPingMsg *pingMsg)
{
    OXLLog("OXLDumpPalPingMsg");
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)logon_buf->base; */
    /* raw */
    OXLLog("pingMsg->palMsg.id = 0x%x", pingMsg->palMsg.id);
    OXLLog("pingMsg->palMsg.len = %d", pingMsg->palMsg.len);
    OXLLog("pingMsg->palMsg.ref = 0x%x", pingMsg->palMsg.ref);
}

void OXLDestroyPalPingMsg(OXLPalPingMsg *pingMsg)
{
    OXLFree(pingMsg);
}