//
//  palgotoroom.h
//  oxyale
//
//  Created by Jesse Riddle on 10/12/15.
//  Copyright © 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palgotoroom_h
#define oxyale_palgotoroom_h

#include "palcom.h"
#include "palclient.h"

typedef struct OXLPalGotoRoomMsgStruct {
    OXLPalMsg msg;
    uint16_t roomId;
} OXLPalGotoRoomMsg;

void OXLDumpPalGotoRoomMsg(OXLPalGotoRoomMsg *gotoRoomMsg);
OXLPalGotoRoomMsg *OXLCreatePalGotoRoomMsg(OXLPalClient *client, uint16_t gotoRoomId);
void OXLInitPalGotoRoomMsg(OXLPalGotoRoomMsg *gotoRoomMsg, OXLPalClient *client);

#endif /* paljoinroom_h */
