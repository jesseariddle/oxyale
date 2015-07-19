//
//  oxyale.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_oxyale_h
#define oxyale_oxyale_h

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#include <libuv/uv.h>
#include <common.h>
#include <palnet.h>
#include <palroom.h>
#include <palreg.h>
#include <palcrypto.h>

#ifndef __cplusplus
#define false 0
#define true 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OXLWriteReqStruct {
    uv_write_t req;
    uv_buf_t buf;
} OXLWriteReq;

void OXLFreeWriteReq(OXLWriteReq *req);
void OXLWriteData(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback);
void OXLDumpBuf(const uv_buf_t buf, size_t size);





/* typedef struct __attribute__((packed)) oxl_net_logon_s { */
typedef struct OXLPalLogonCmdStruct {
    PAL_HEADER_FIELDS
    int32_t regCRC;
    int32_t regCounter;
    uint8_t usernameLen;
    char username[PAL_USERNAME_SZ_CAP];
    char wizpass[PAL_WIZ_PASS_SZ_CAP];
    int32_t flags;
    int32_t PUIDCounter;
    int32_t PUIDCRC;
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

typedef struct OXLPalServerInfoMsgStruct {
    PAL_HEADER_FIELDS
    int32_t permissions;
    uint8_t size;
    char servername[PAL_SERVER_NAME_SZ_CAP];
} OXLPalServerInfoMsg;

void OXLPalInitLogonCmd(OXLPalLogonCmd *logonCmd,
                         char *username,
                         char *wizpass,
                         uint32_t initialRoomID,
                         uint32_t regCRC,
                         uint32_t regCounter,
                         uint16_t PUIDCRC,
                         uint32_t PUIDCounter);

void OXLPalDumpLogonCmd(const OXLPalLogonCmd *logonCmd);

/* init client */
/* void oxl_client_init(oxl_client_t *self, uv_loop_t *loop); */
/* start client */
void OXLPalOpenConnection(OXLPalClient *client,
                           uv_loop_t *loop,
                           char *username,
                           char *wizpass,
                           char *host,
                           uint16_t port,
                           int32_t initialRoom);

void OXLPalCloseConnection(OXLPalClient *client);

void OXLPalSay(OXLPalClient *client, size_t size, uv_buf_t buf, uv_write_cb finishSay);

/* Room Section */
typedef struct OXLPalRoomMsgStruct {
    PAL_HEADER_FIELDS
    int32_t roomFlags;
    int32_t face;
    int16_t roomID;
    int16_t roomNameOffset;
    int16_t imageNameOffset;
    int16_t artistNameOffset;
    int16_t passwordOffset;
    int16_t hotSpotCount;
    int16_t hotSpotOffset;
    int16_t imageCount;
    int16_t imageOffset;
    int16_t drawCommandsCount;
    int16_t firstDrawCommandOffset;
    int16_t peopleCount;
    int16_t loosePropCount;
    int16_t firstLoosePropOffset;
    int16_t reserved;
    int16_t dataLen;
    byte *data;
} OXLPalRoomMsg;

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

void OXLPalRoomMsgToRoom(OXLPalRoom *room, const byte *data, const uint refID);

/*
uint oxl_net_room_len(const unsigned char *data);
short oxl_net_room_data_len(const unsigned char *data);
short oxl_net_room_room_name_len(const unsigned char *data);
short oxl_net_room_image_name_len(const unsigned char *data);
void oxl_net_room_room_name(char *room_name,
                            const oxl_net_room_t *room_msg);

void oxl_net_room_image_name(char *image_name,
                             const oxl_net_room_t *room_msg);
*/

void OXLPalInitRoom(OXLPalRoom *room);
void OXLPalFinishSelectedUserChanged(OXLPalUser *user);

typedef struct OXLPalGotoRoomCmdStruct {
    PAL_HEADER_FIELDS
    uint16_t roomID;
} OXLPalGotoRoomCmd;

void OXLPalInitGotoRoomCmd(OXLPalGotoRoomCmd *room, uint userID, short gotoRoomID);
void OXLPalLeaveRoomCmd(OXLPalClient *client, OXLPalRoom *room);
void OXLPalJoinRoomCmd(OXLPalClient *client, uint gotoRoomID, void *finishJoinRoom);
/* void oxl_client_goto_room(oxl_client_t *self, uint goto_room_id); */

/* don't export */
#undef PAL_HEADER_FIELDS

#ifdef __cplusplus
}
#endif
#endif /* OXYALE_H_ */
