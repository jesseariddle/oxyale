//
//  palcom.c
//  oxyale
//
//  Created by Jesse Riddle on 10/12/15.
//  Copyright © 2015 Riddle Software. All rights reserved.
//

#include <palcom.h>

void OXLInitPalMsg(OXLPalMsg *palMsg) {
    palMsg->id = 0;
    palMsg->len = 0;
    palMsg->ref = 0;
}

void OXLDumpPalMsg(const OXLPalMsg *palMsg) {
    char msgText[5] = { '\0' };
    memcpy(&msgText, &palMsg->id, 4);
    msgText[4] = '\0';
    OXLLogDebug("palMsg->id = 0x%x (%s)", palMsg->id, msgText);
    OXLLogDebug("palMsg->len = 0x%x (%u)", palMsg->len, palMsg->len);
    OXLLogDebug("palMsg->ref = 0x%x", palMsg->ref);
}