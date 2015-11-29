//
//  oxyale-palroom.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palroom_h
#define oxyale_palroom_h

#include "paluser.h"
#include "palhotspot.h"
#include "palprop.h"
#include "palview.h"

typedef struct OXLPalRoomMsgStruct {
    OXLPalMsg palMsg;
    int32_t roomFlags;
    int32_t face;
    int16_t roomID;
    int16_t roomNameOffset;
    int16_t imageNameOffset;
    int16_t artistNameOffset;
    int16_t passwordOffset;
    int16_t hotSpotCount;
    int16_t hotSpotOffset;
    int16_t imageCount;
    int16_t imageOffset;
    int16_t drawCommandsCount;
    int16_t firstDrawCommandOffset;
    int16_t peopleCount;
    int16_t loosePropCount;
    int16_t firstLoosePropOffset;
    int16_t reserved;
    int16_t dataLen;
    byte *data;
} OXLPalRoomMsg;

typedef struct OXLPalRoomStruct {
    int32_t id;
    char *name;
    char *backgroundFilename;
    OXLPalUser *users;
    void *usersHash;
    int32_t roomFlags;
    void *spotImages;
    OXLPalHotSpot *hotspots;
    OXLPalHotSpot *hotspotsAboveNothing;
    OXLPalHotSpot *hotspotsAboveAvatars;
    OXLPalHotSpot *hotspotsAboutNametags;
    OXLPalHotSpot *hotspotsAboveEverything;
    void *hotspotsById;
    OXLPalProp *looseProps;
    void *drawFrontCommands;
    void *drawBackCommands;
    void *drawLayerHistory;
    OXLPalUser *selectedUser;
    int32_t userId;
    OXLPalView *roomView;
    double dimLevel;
    int32_t showAvatarsFlag;
    char *chatLog;
    char *lastMessage;
    int32_t lastMessageCount;
    int32_t lastMessageReceived;
    int32_t lastMessageTimer;
    char *statusMessage;
    int32_t statusDisappearTimer;
} OXLPalRoom;

/*
 uint oxl_net_room_len(const unsigned char *data);
 short oxl_net_room_data_len(const unsigned char *data);
 short oxl_net_room_room_name_len(const unsigned char *data);
 short oxl_net_room_image_name_len(const unsigned char *data);
 void oxl_net_room_room_name(char *room_name,
 const oxl_net_room_t *room_msg);
 
 void oxl_net_room_image_name(char *image_name,
 const oxl_net_room_t *room_msg);
 */

void OXLInitPalRoom(OXLPalRoom *room);
void OXLPalFinishSelectedUserChanged(OXLPalUser *user);

/* void OXLCreatePalMsgToRoom(OXLPalRoom *room, const byte *data, const uint refID); */
/* void */

#endif
