//
//  pallogon.h
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_pallogon_h
#define oxyale_pallogon_h

#include <oxlcom.h>

#define PAL_USERNAME_SZ_CAP 31
#define PAL_WIZ_PASS_SZ_CAP 32
#define PAL_SERVER_NAME_SZ_CAP 256
#define PAL_RESERVED_LOGON_LEN 5

/* typedef struct __attribute__((packed)) oxl_net_logon_s { */
typedef struct OXLPalLogonCmdStruct {
    int32_t msgID;
    int32_t msgLen;
    int32_t msgRef;
    int32_t regCRC;
    int32_t regCounter;
    uint8_t usernameLen;
    char username[PAL_USERNAME_SZ_CAP];
    char wizpass[PAL_WIZ_PASS_SZ_CAP];
    int32_t flags;
    int32_t puidCounter;
    int32_t puidCRC;
    int32_t demoElapsed;
    int32_t totalElapsed;
    int32_t demoLimit;
    int16_t initialRoomID;
    char reserved[PAL_RESERVED_LOGON_LEN];
    int32_t ulUploadRequestedProtocolVersion;
    int32_t ulUploadCapabilities;
    int32_t ulDownloadCapabilities;
    int32_t ul2DEngineCapabilities;
    int32_t ul2DGraphicsCapabilities;
    int32_t ul3DEngineCapabilities;
} OXLPalLogonCmd;

void OXLPalLogonCmdInit(OXLPalLogonCmd *logonCmd,
                        char *username,
                        char *wizpass,
                        uint32_t initialRoomID,
                        uint32_t regCRC,
                        uint32_t regCounter,
                        uint16_t puidCRC,
                        uint32_t puidCounter);

OXLPalLogonCmd *OXLPalLogonCmdCreate(char *username,
                                     char *wizpass,
                                     uint32_t initialRoomID,
                                     uint32_t regCRC,
                                     uint32_t regCounter,
                                     uint16_t puidCRC,
                                     uint32_t puidCounter);

void OXLPalLogonCmdFree(OXLPalLogonCmd logonCmd);
void OXLPalLogonCmdDump(const OXLPalLogonCmd *logonCmd);

#endif
