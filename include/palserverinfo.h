//
//  palserverinfo.h
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palserverinfo_h
#define oxyale_palserverinfo_h

#include "palcom.h"

typedef struct OXLPalServerInfoMsgStruct {
    OXLPalMsg palMsg;
    int32_t permissions;
    uint8_t size;
    char servername[PAL_SERVER_NAME_SZ_CAP];
} OXLPalServerInfoMsg;

#endif
