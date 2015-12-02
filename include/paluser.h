//
//  paluser.h
//  oxyale
//
//  Created by Jesse Riddle on 7/19/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_paluser_h
#define oxyale_paluser_h

#include "oxlcom.h"
#include "palcom.h"
#include "palpropstore.h"

#define OXL_PAL_MAX_USERS           400
#define OXL_PAL_MAX_USER_ID         10000

#define UF_WIZARD           0x0001
#define UF_SYSOP            0x0002
#define UF_KILL             0x0004
#define UF_GUEST            0x0008
#define UF_BANISHED         0x0010
#define UF_PENALIZED        0x0020
#define UF_COMMERROR        0x0040
#define UF_GAG              0x0080
#define UF_PIN              0x0100
#define UF_HIDE             0x0200
#define UF_REJECTESP        0x0400
#define UF_REJECTPRIVATE    0x0800
#define UF_PROPGAG          0x1000

/*
typedef enum UserFlagsEnum {
    UF_WIZARD = 0x0001,
    UF_SYSOP = 0x0002,
    UF_KILLED = 0x0004,
    UF_GUEST = 0x0008,
    UF_BANISHED = 0x0010,
    UF_PENALIZED = 0x0020,
    UF_COMMERROR = 0x0040,
    UF_GAG = 0x0080,
    UF_PIN = 0x0100,
    UF_HIDE = 0x0200,
    UF_REJECTESP = 0x0400,
    UF_REJECTPRIVATE = 0x0800,
    UF_PROPGAG = 0x1000,
} UserFlags;
*/

typedef struct OXLPalUserStruct {
    OXLPalMsg msg;
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

typedef struct OXLPalUserListItemStruct {
    int32_t id;
    int16_t flags;
    int16_t roomId;
    OXLP8String name;
} OXLPalUserListItem;

#endif
