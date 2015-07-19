//
//  paluser.h
//  oxyale
//
//  Created by Jesse Riddle on 7/19/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_paluser_h
#define oxyale_paluser_h

typedef struct OXLPalUserStruct {
    PAL_HEADER_FIELDS
    int32_t id;
    char name[PAL_USERNAME_SZ_CAP]; /* 32 */
    int32_t x;
    int32_t y;
    int32_t roomID;
    char roomName[PAL_ROOM_NAME_SZ_CAP]; /* 256 */
    int32_t propCount;
    int32_t face;
    int32_t color;
    int32_t flags;
    /* int prop_ids[256]; */
    /* int prop_crcs[256]; */
    /* oxl_client_prop_t props[256]; */
    void *propIDs;
    void *propCRCs;
    void *props;
    int32_t showFaceFlag;
    OXLPalPropStore propStore;
} OXLPalUser;

#endif
