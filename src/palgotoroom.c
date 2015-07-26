//
//  palgotoroom.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <stdio.h>
#include <palclient.h>
#include <palgotoroom.h>

void OXLPalGotoRoomCmdInit(OXLPalGotoRoomCmd *gotoRoomCmd, uint userID, short roomID)
{
    gotoRoomCmd->msgID = PAL_TX_GOTO_ROOM_CMD;
    gotoRoomCmd->msgLen = sizeof(short);
    gotoRoomCmd->msgRef = userID;
    gotoRoomCmd->roomID = roomID;
}