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
    OXLLog("palMsg->id = %d", palMsg->id);
    OXLLog("palMsg->len = %d", palMsg->len);
    OXLLog("palMsg->ref = %d", palMsg->ref);
}