//
//  palgotoroom.h
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palgotoroom_h
#define oxyale_palgotoroom_h

typedef struct OXLPalGotoRoomMsgStruct {
    OXLPalMsg palMsg;
    uint16_t roomId;
} OXLPalGotoRoomMsg;

void OXLInitPalGotoRoomMsg(OXLPalGotoRoomMsg *gotoRoomMsg, uint32_t userId, uint16_t gotoRoomId);
/* void oxl_client_goto_room(oxl_client_t *self, uint goto_room_id); */
OXLPalGotoRoomMsg *OXLCreatePalGotoRoomMsg(uint32_t userId, uint16_t gotoRoomId);
void OXLDestroyPalGotoRoomMsg(OXLPalGotoRoomMsg *gotoRoomMsg);

#endif
