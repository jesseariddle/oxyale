//
//  palgotoroom.h
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palgotoroom_h
#define oxyale_palgotoroom_h

typedef struct OXLPalGotoRoomCmdStruct {
    PAL_HEADER_FIELDS
    uint16_t roomID;
} OXLPalGotoRoomCmd;

void OXLPalGotoRoomCmdInit(OXLPalGotoRoomCmd *room, uint userID, short gotoRoomID);

/* void oxl_client_goto_room(oxl_client_t *self, uint goto_room_id); */

#endif
