//
//  pallogon.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <pallogon.h>

void OXLInitPalLogonCmd(OXLPalLogonCmd *logonCmd,
                        char *username,
                        char *wizpass,
                        uint32_t initialRoomID,
                        uint32_t regCRC,
                        uint32_t regCounter,
                        uint16_t puidCRC,
                        uint32_t puidCounter)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)buf->base; */
    OXLLog("PalLogonCmdInit");
    logonCmd->msgID = PAL_TX_LOGON_CMD;
    logonCmd->msgLen = 0x80;
    logonCmd->msgRef = 0;
    logonCmd->regCRC = regCRC;
    logonCmd->regCounter = regCounter;
    logonCmd->usernameLen = (uint8_t)strlen(username);
    strncpy(logonCmd->username, username, PAL_USERNAME_SZ_CAP);
    strncpy(logonCmd->wizpass, wizpass, PAL_WIZ_PASS_SZ_CAP);
    
    /* raw */
    logonCmd->flags = PAL_AUXFLAGS_AUTHENTICATE | PAL_AUXFLAGS_WIN32;
    
    logonCmd->puidCounter = puidCounter;
    logonCmd->puidCRC = puidCRC;
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
        PAL_DLCAPS_ASSETS_PALACE | PAL_DLCAPS_FILES_PALACE | PAL_DLCAPS_FILES_HTTPSRVR;
    
    /* unused */
    logonCmd->ul2DEngineCapabilities = 0;
    
    /* unused */
    logonCmd->ul2DGraphicsCapabilities = 0;
    
    /* unused */
    logonCmd->ul3DEngineCapabilities = 0;
}

OXLPalLogonCmd *OXLMakePalLogonCmd(char *username,
                                   char *wizpass,
                                   uint32_t initialRoomID,
                                   uint32_t regCRC,
                                   uint32_t regCounter,
                                   uint16_t puidCRC,
                                   uint32_t puidCounter)
{
    OXLLog("PalLogonCmdCreate");
    OXLPalLogonCmd *logonCmd = OXLAlloc(sizeof(*logonCmd));
    OXLInitPalLogonCmd(logonCmd,
                       username,
                       wizpass,
                       initialRoomID,
                       regCRC,
                       regCounter,
                       puidCRC,
                       puidCounter);
    return logonCmd;
}

void OXLDumpPalLogonCmd(const OXLPalLogonCmd *logonCmd)
{
    OXLLog("PalLogonCmdDump");
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)logon_buf->base; */
    /* raw */
    OXLLog("logonCmd->msgID = 0x%x", logonCmd->msgID);
    OXLLog("logonCmd->msgSize = %d", logonCmd->msgLen);
    OXLLog("logonCmd->msgRef = 0x%x", logonCmd->msgRef);
    OXLLog("logonCmd->regCRC = 0x%x", logonCmd->regCRC);
    OXLLog("logonCmd->regCounter = 0x%x", logonCmd->regCounter);
    OXLLog("logonCmd->usernameLen = %d", logonCmd->usernameLen);
    OXLLog("logonCmd->username = %s", logonCmd->username);
    OXLLog("logonCmd->flags = 0x%x", logonCmd->flags);
    OXLLog("logonCmd->puidCounter = 0x%x", logonCmd->puidCounter);
    OXLLog("logonCmd->puidCRC = 0x%x", logonCmd->puidCRC);
    OXLLog("logonCmd->demoElapsed = 0x%x", logonCmd->demoElapsed);
    OXLLog("logonCmd->totalElapsed = 0x%x", logonCmd->totalElapsed);
    OXLLog("logonCmd->demoLimit = 0x%x", logonCmd->demoLimit);
    
    /* initial room */
    OXLLog("logonCmd->roomID = %d", logonCmd->initialRoomID);
    
    /* does nothing, but is logged by server */
    OXLLog("logonCmd->reserved = %s", logonCmd->reserved);
    
    /* ignored on server */
    OXLLog("logonCmd->ulUploadRequestedProtocolVersion = 0x%x",
           logonCmd->ulUploadRequestedProtocolVersion);
    
    /* TODO upload capabilities plox */
    OXLLog("logonCmd->ulUploadCapabilities = 0x%x", logonCmd->ulUploadCapabilities);
    
    /* TODO download capabilities plox */
    OXLLog("logonCmd->ulDownloadCapabilities = 0x%x", logonCmd->ulDownloadCapabilities);
    
    /* unused */
    OXLLog("logonCmd->ul2DEngineCapabilities = 0x%x", logonCmd->ul2DEngineCapabilities);
    
    /* unused */
    OXLLog("logonCmd->ul2DGraphicsCapabilities = 0x%x", logonCmd->ul2DGraphicsCapabilities);
    
    /* unused */
    OXLLog("logonCmd->ul3DEngineCapabilities = 0x%x", logonCmd->ul3DEngineCapabilities);
}

void OXLReleasePalLogonCmd(OXLPalLogonCmd *logonCmd)
{
    OXLRelease(logonCmd);
}