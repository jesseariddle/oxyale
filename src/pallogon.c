//
//  pallogon.c
//  oxyale
//
//  Created by Jesse Riddle on 7/21/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <palclient.h>
#include <pallogon.h>

void OXLInitPalLogonMsg(OXLPalLogonMsg *logonMsg,
                        const char *username,
                        const char *wizpass,
                        const uint32_t initialRoomId,
                        const uint32_t regCRC,
                        const uint32_t regCounter,
                        const uint16_t puidCRC,
                        const uint32_t puidCounter)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)buf->base; */
    OXLLogDebug("OXLInitPalLogonMsg");
    
    OXLPalLogonMsg logonMsgInit = {
        .palMsg.id = PAL_TX_LOGON_MSG,
        .palMsg.len = 0x80,
        .palMsg.ref = 0,
        .regCRC = regCRC,
        .regCounter = regCounter,
        .usernameLen = (uint8_t)strnlen(username, PAL_USERNAME_SZ_CAP),
        .flags =
            PAL_AUXFLAGS_AUTHENTICATE |
            PAL_AUXFLAGS_WIN32,
        .puidCounter = puidCounter,
        .puidCRC = puidCRC,
        .demoElapsed = 0x00011940, /* 0 */
        .totalElapsed = 0x00011940, /* 0 */
        .demoLimit = 0x00011940, /* 0 */
        .initialRoomId = initialRoomId,
        .ulUploadRequestedProtocolVersion = 0,
        .ulUploadCapabilities = PAL_LI_ULCAPS_ASSETS_PALACE | PAL_LI_ULCAPS_FILES_HTTP,
        .ulDownloadCapabilities = PAL_LI_DLCAPS_ASSETS_PALACE | PAL_LI_DLCAPS_FILES_PALACE | PAL_LI_DLCAPS_FILES_HTTP | PAL_LI_DLCAPS_FILES_HTTP_EXTENDED,
        .ul2DEngineCapabilities = PAL_LI_2DENGINECAP_PALACE,
        .ul2DGraphicsCapabilities = PAL_LI_2DGRAPHCAP_GIF87,
        .ul3DEngineCapabilities = 0
    };

    strncpy((char *)&logonMsgInit.username, username, PAL_USERNAME_SZ_CAP); /* PString */
    strlcpy((char *)&logonMsgInit.wizpass, wizpass, PAL_WIZ_PASS_SZ_CAP); /* CString */
    
    /* does nothing, but is logged by server */
    /* strncpy(logonCmd->reserved, "OXYALE", 6); */
    strncpy((char *)&logonMsgInit.reserved, "PC4235", PAL_RESERVED_LOGON_LEN);
    /* strncpy((char *)&logonMsgInit.reserved, "OPNPAL", PAL_RESERVED_LOGON_LEN);*/

    memcpy(logonMsg, &logonMsgInit, sizeof(*logonMsg));
}

OXLPalLogonMsg *OXLCreatePalLogonMsg(const char *username,
                                     const char *wizpass,
                                     const uint32_t initialRoomId,
                                     const uint32_t regCRC,
                                     const uint32_t regCounter,
                                     const uint16_t puidCRC,
                                     const uint32_t puidCounter)
{
    OXLLogDebug("OXLCreatePalLogonMsg");
    OXLPalLogonMsg *logonMsg = OXLAlloc(sizeof(*logonMsg));
    OXLInitPalLogonMsg(logonMsg,
                       username,
                       wizpass,
                       initialRoomId,
                       regCRC,
                       regCounter,
                       puidCRC,
                       puidCounter);
    return logonMsg;
}

void OXLDumpPalLogonMsg(const OXLPalLogonMsg *logonMsg)
{
    OXLLogDebug("OXLDumpPalLogonMsg");
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)logon_buf->base; */
    /* raw */
    OXLLogDebug("logonMsg->palMsg.id = 0x%x", logonMsg->palMsg.id);
    OXLLogDebug("logonMsg->palMsg.len = %d", logonMsg->palMsg.len);
    OXLLogDebug("logonMsg->palMsg.ref = 0x%x", logonMsg->palMsg.ref);
    OXLLogDebug("logonMsg->regCRC = 0x%x", logonMsg->regCRC);
    OXLLogDebug("logonMsg->regCounter = 0x%x", logonMsg->regCounter);
    OXLLogDebug("logonMsg->usernameLen = %d", logonMsg->usernameLen);
    OXLLogDebug("logonMsg->username = %s", logonMsg->username);
    OXLLogDebug("logonMsg->flags = 0x%x", logonMsg->flags);
    OXLLogDebug("logonMsg->puidCounter = 0x%x", logonMsg->puidCounter);
    OXLLogDebug("logonMsg->puidCRC = 0x%x", logonMsg->puidCRC);
    OXLLogDebug("logonMsg->demoElapsed = 0x%x", logonMsg->demoElapsed);
    OXLLogDebug("logonMsg->totalElapsed = 0x%x", logonMsg->totalElapsed);
    OXLLogDebug("logonMsg->demoLimit = 0x%x", logonMsg->demoLimit);
    
    /* initial room */
    OXLLogDebug("logonMsg->roomId = %d", logonMsg->initialRoomId);
    
    /* does nothing, but is logged by server */
    OXLLogDebug("logonMsg->reserved = %s", logonMsg->reserved);
    
    /* ignored on server */
    OXLLogDebug("logonMsg->ulUploadRequestedProtocolVersion = 0x%x",
           logonMsg->ulUploadRequestedProtocolVersion);
    
    /* TODO upload capabilities plox */
    OXLLogDebug("logonMsg->ulUploadCapabilities = 0x%x", logonMsg->ulUploadCapabilities);
    
    /* TODO download capabilities plox */
    OXLLogDebug("logonMsg->ulDownloadCapabilities = 0x%x", logonMsg->ulDownloadCapabilities);
    
    /* unused */
    OXLLogDebug("logonMsg->ul2DEngineCapabilities = 0x%x", logonMsg->ul2DEngineCapabilities);
    
    /* unused */
    OXLLogDebug("logonMsg->ul2DGraphicsCapabilities = 0x%x", logonMsg->ul2DGraphicsCapabilities);
    
    /* unused */
    OXLLogDebug("logonMsg->ul3DEngineCapabilities = 0x%x", logonMsg->ul3DEngineCapabilities);
}

void OXLDestroyPalLogonMsg(OXLPalLogonMsg *logonMsg)
{
    OXLFree(logonMsg);
}