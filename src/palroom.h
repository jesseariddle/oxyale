//
//  oxyale-palroom.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_oxyale_palroom_h
#define oxyale_oxyale_palroom_h

typedef struct OXLPalHotUserStruct OXLPalUser;
typedef struct OXLPalHotSpotStruct OXLPalHotSpot;
typedef struct OXLPalPropStruct OXLPalProp;
typedef struct OXLPalViewStruct OXLPalView;

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
    void *hotspotsByID;
    OXLPalProp *looseProps;
    void *drawFrontCommands;
    void *drawBackCommands;
    void *drawLayerHistory;
    OXLPalUser *selectedUser;
    int32_t userID;
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

#endif
