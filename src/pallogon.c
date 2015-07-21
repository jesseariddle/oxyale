//
//  pallogon.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <pallogon.h>

void OXLPalLogonCmdInit(OXLPalLogonCmd *logonCmd,
                        char *username,
                        char *wizpass,
                        int initialRoomID,
                        uint regCRC,
                        int regCounter,
                        uint PUIDCRC,
                        int PUIDCounter)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)buf->base; */
    fprintf(stderr, "--- DEBUG: InitPalLogonCmd\n");
    logonCmd->msgID = PAL_TX_LOGON_CMD;
    logonCmd->msgLen = 0x80;
    logonCmd->msgRef = 0;
    logonCmd->regCRC = regCRC;
    logonCmd->regCounter = regCounter;
    logonCmd->usernameLen = (byte)strlen(username);
    strncpy(logonCmd->username, username, PAL_USERNAME_SZ_CAP);
    strncpy(logonCmd->wizpass, wizpass, PAL_WIZ_PASS_SZ_CAP);
    
    /* raw */
    logonCmd->flags = PAL_AUXFLAGS_AUTHENTICATE | PAL_AUXFLAGS_WIN32;
    
    logonCmd->puidCounter = PUIDCounter;
    logonCmd->puidCRC = PUIDCRC;
    logonCmd->demoElapsed = 0;
    logonCmd->totalElapsed = 0;
    logonCmd->demoLimit = 0;
    
    /* initial room */
    logonCmd->initialRoomID = initialRoomID;
    
    /* does nothing, but is logged by server */
    /* strncpy(logonCmd->reserved, "OXYALE", 6); */
    /* strncpy(logonCmd->reserved, "PC4232", 6); */
    strncpy(logonCmd->reserved, "OPNPAL", 6);
    
    /* ignored on server */
    logonCmd->ulUploadRequestedProtocolVersion = 0;
    
    /* TODO upload capabilities plox */
    logonCmd->ulUploadCapabilities = PAL_ULCAPS_ASSETS_PALACE;
    
    /* TODO download capabilities plox */
    logonCmd->ulDownloadCapabilities =
    PAL_DLCAPS_ASSETS_PALACE |
    PAL_DLCAPS_FILES_PALACE |
    PAL_DLCAPS_FILES_HTTPSRVR;
    
    /* unused */
    logonCmd->ul2DEngineCapabilities = 0;
    
    /* unused */
    logonCmd->ul2DGraphicsCapabilities = 0;
    
    /* unused */
    logonCmd->ul3DEngineCapabilities = 0;
}

void OXLPalLogonCmdDump(const OXLPalLogonCmd *logonCmd)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)logon_buf->base; */
    /* raw */
    fprintf(stderr, "logonCmd->msgID = 0x%x\n", logonCmd->msgID);
    fprintf(stderr, "logonCmd->msgSize = %d\n", logonCmd->msgLen);
    fprintf(stderr, "logonCmd->msgRef = 0x%x\n", logonCmd->msgRef);
    fprintf(stderr, "logonCmd->regCRC = 0x%x\n", logonCmd->regCRC);
    fprintf(stderr, "logonCmd->regCounter = 0x%x\n", logonCmd->regCounter);
    fprintf(stderr, "logonCmd->usernameLen = %d\n", logonCmd->usernameLen);
    fprintf(stderr, "logonCmd->username = %s\n", logonCmd->username);
    fprintf(stderr, "logonCmd->flags = 0x%x\n", logonCmd->flags);
    fprintf(stderr, "logonCmd->puidCounter = 0x%x\n", logonCmd->puidCounter);
    fprintf(stderr, "logonCmd->puidCRC = 0x%x\n", logonCmd->puidCRC);
    fprintf(stderr, "logonCmd->demoElapsed = 0x%x\n", logonCmd->demoElapsed);
    fprintf(stderr, "logonCmd->totalElapsed = 0x%x\n", logonCmd->totalElapsed);
    fprintf(stderr, "logonCmd->demoLimit = 0x%x\n", logonCmd->demoLimit);
    
    /* initial room */
    fprintf(stderr, "logonCmd->roomID = %d\n", logonCmd->initialRoomID);
    
    /* does nothing, but is logged by server */
    fprintf(stderr, "logonCmd->reserved = %s\n", logonCmd->reserved);
    
    /* ignored on server */
    fprintf(stderr,
            "logonCmd->ulUploadRequestedProtocolVersion = 0x%x\n",
            logonCmd->ulUploadRequestedProtocolVersion);
    
    /* TODO upload capabilities plox */
    fprintf(stderr, "logonCmd->ulUploadCapabilities = 0x%x\n", logonCmd->ulUploadCapabilities);
    
    /* TODO download capabilities plox */
    fprintf(stderr, "logonCmd->ulDownloadCapabilities = 0x%x\n", logonCmd->ulDownloadCapabilities);
    
    /* unused */
    fprintf(stderr, "logonCmd->ul2DEngineCapabilities = 0x%x\n", logonCmd->ul2DEngineCapabilities);
    
    /* unused */
    fprintf(stderr, "logonCmd->ul2DGraphicsCapabilities = 0x%x\n", logonCmd->ul2DGraphicsCapabilities);
    
    /* unused */
    fprintf(stderr, "logonCmd->ul3DEngineCapabilities = 0x%x\n", logonCmd->ul3DEngineCapabilities);
}