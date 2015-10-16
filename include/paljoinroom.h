//
//  paljoinroom.h
//  oxyale
//
//  Created by Jesse Riddle on 10/12/15.
//  Copyright © 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_paljoinroom_h
#define oxyale_paljoinroom_h

#include "palcom.h"
#include "palclient.h"

typedef struct OXLPalJoinRoomMsgStruct {
    OXLPalMsg msg;
} OXLPalJoinRoomMsg;

void OXLDumpPalJoinRoomMsg(OXLPalJoinRoomMsg *joinRoomMsg);
OXLPalJoinRoomMsg *OXLCreatePalJoinRoomMsg(OXLPalClient *client, uint16_t gotoRoomId);
void OXLInitPalJoinRoomMsg(OXLPalJoinRoomMsg *joinRoomMsg, OXLPalClient *client);

#endif /* paljoinroom_h */
