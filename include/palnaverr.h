//
//  palnaverr.h
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palnaverr_h
#define oxyale_palnaverr_h

/* nav messages */
typedef enum OXLPalNavErrTypeEnum {
    NAV_ERR_INTERNAL_ERROR,
    NAV_ERR_ROOM_UNKNOWN,
    NAV_ERR_ROOM_FULL,
    NAV_ERR_ROOM_CLOSED,
    NAV_ERR_CANT_AUTHOR,
    NAV_ERR_PALACE_FULL
} OXLPalNavErrType;

typedef struct OXLPalNavErrStruct {
    int32_t refID;
    int16_t code;
} OXLPalNavErr;

void OXLPalInitNavErr(OXLPalNavErr *navErr, const byte *data, const uint refID);

#endif
