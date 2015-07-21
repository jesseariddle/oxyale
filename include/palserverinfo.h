//
//  palserverinfo.h
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palserverinfo_h
#define oxyale_palserverinfo_h

typedef struct OXLPalServerInfoMsgStruct {
    PAL_HEADER_FIELDS
    int32_t permissions;
    uint8_t size;
    char servername[PAL_SERVER_NAME_SZ_CAP];
} OXLPalServerInfoMsg;

#endif
