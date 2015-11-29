//
//  palgotoroom.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <stdio.h>
#include <palcom.h>
#include <palgotoroom.h>

void
OXLInitPalGotoRoomMsg(OXLPalGotoRoomMsg *gotoRoomMsg, uint32_t userId, uint16_t roomId)
{
    gotoRoomMsg->palMsg.id = PAL_TX_GOTO_ROOM_MSG;
    gotoRoomMsg->palMsg.len = sizeof(roomId);
    gotoRoomMsg->palMsg.ref = userId;
    gotoRoomMsg->roomId = roomId;
}

OXLPalGotoRoomMsg *
OXLCreatePalGotoRoomMsg(uint32_t userId, uint16_t roomId)
{
    OXLPalGotoRoomMsg *gotoRoomMsg = OXLAlloc(sizeof(*gotoRoomMsg));
    OXLInitPalGotoRoomMsg(gotoRoomMsg, userId, roomId);
    return gotoRoomMsg;
}

void
OXLDestroyPalGotoRoomMsg(OXLPalGotoRoomMsg *gotoRoomMsg)
{
    OXLFree(gotoRoomMsg);
}
