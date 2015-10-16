//
//  palleaveroom.h
//  oxyale
//
//  Created by Jesse Riddle on 10/12/15.
//  Copyright © 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palleaveroom_h
#define oxyale_palleaveroom_h

#include "palcom.h"
#include "palclient.h"

typedef struct OXLPalLeaveRoomMsgStruct {
    OXLPalMsg msg;
    
} OXLPalLeaveRoomMsg;

void OXLDumpPalLeaveRoomMsg(OXLPalLeaveRoomMsg *leaveRoomMsg);
OXLPalLeaveRoomMsg *OXLCreatePalLeaveRoomMsg(OXLPalClient *client);
void OXLInitPalLeaveRoomMsg(OXLPalLeaveRoomMsg *leaveRoomMsg, OXLPalClient *client);

#endif /* palleaveroom_h */
