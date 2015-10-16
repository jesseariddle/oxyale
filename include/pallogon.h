//
//  pallogon.h
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_pallogon_h
#define oxyale_pallogon_h

#include "palcom.h"

#define PAL_USERNAME_SZ_CAP 31
#define PAL_WIZ_PASS_SZ_CAP 32
#define PAL_SERVER_NAME_SZ_CAP 256
#define PAL_RESERVED_LOGON_LEN 5

/* typedef struct __attribute__((packed)) oxl_net_logon_s { */
typedef struct OXLPalLogonMsgStruct {
    const OXLPalMsg palMsg;
    const int32_t regCRC;
    const int32_t regCounter;
    const uint8_t usernameLen;
    const char username[PAL_USERNAME_SZ_CAP];
    const char wizpass[PAL_WIZPASS_SZ_CAP];
    const int32_t flags;
    const int32_t puidCounter;
    const int32_t puidCRC;
    const int32_t demoElapsed;
    const int32_t totalElapsed;
    const int32_t demoLimit;
    const int16_t initialRoomId;
    const char reserved[PAL_RESERVED_LOGON_LEN];
    const int32_t ulUploadRequestedProtocolVersion;
    const int32_t ulUploadCapabilities;
    const int32_t ulDownloadCapabilities;
    const int32_t ul2DEngineCapabilities;
    const int32_t ul2DGraphicsCapabilities;
    const int32_t ul3DEngineCapabilities;
} OXLPalLogonMsg;

void OXLInitPalLogonMsg(OXLPalLogonMsg *logonMsg,
                        const char *username,
                        const char *wizpass,
                        const uint32_t initialRoomId,
                        const uint32_t regCRC,
                        const uint32_t regCounter,
                        const uint16_t puidCRC,
                        const uint32_t puidCounter);

OXLPalLogonMsg *OXLCreatePalLogonMsg(const char *username,
                                     const char *wizpass,
                                     const uint32_t initialRoomID,
                                     const uint32_t regCRC,
                                     const uint32_t regCounter,
                                     const uint16_t puidCRC,
                                     const uint32_t puidCounter);

void OXLDestroyPalLogonMsg(OXLPalLogonMsg *logonMsg);
void OXLDumpPalLogonMsg(const OXLPalLogonMsg *logonMsg);

#endif
