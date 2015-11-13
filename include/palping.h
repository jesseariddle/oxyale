//
//  palping.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palping_h
#define oxyale_palping_h

#include "palcom.h"

typedef struct OXLPalPingMsgStruct {
    const OXLPalMsg palMsg;
} OXLPalPingMsg;

void OXLInitPalPingMsg(OXLPalPingMsg *pingMsg);
void OXLDumpPalPingMsg(const OXLPalPingMsg *pingMsg);
OXLPalPingMsg *OXLCreatePalPingMsg(void);
void OXLDestroyPalPingMsg(OXLPalPingMsg *pingMsg);

extern OXLPalPingMsg const kPalPingMsg;

#endif
