#pragma once
#ifndef OXYALE_H
#define OXYALE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <libuv/uv.h>
#include <oxyale-defs.h>

#ifndef __cplusplus
#define false 0
#define true 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;
typedef unsigned int uint;

#define PAL_HEADER_FIELDS \
/* public */ \
uint msgID; \
uint msgLen; \
uint msgRef; \

typedef struct OXLPalHeaderStruct {
    PAL_HEADER_FIELDS
} OXLPalHeader;

/*
extern uint8_t oxl_env_is_big_endian();
extern uint32_t oxl_uint32_swap_endian(const uint32_t n);
extern void oxl_strify(char *str, const uint32_t uint32);
*/
    
typedef struct OXLWriteReqStruct {
    uv_write_t req;
    uv_buf_t buf;
} OXLWriteReq;

void OXLFreeWriteReq(OXLWriteReq *req);
void OXLWriteData(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback);

void OXLDumpPalBuf(const uv_buf_t buf, size_t size);

typedef struct OXLPalPropStruct {
    /* stub */
} OXLPalProp;

typedef struct OXLPalPropStoreStruct {

} OXLPalPropStore;

typedef struct OXLPalCryptStruct {
    int32_t seed;
    byte lut[PAL_CRYPT_LUT_LEN];
} OXLPalCrypt;

typedef struct OXLPalSayBubbleStruct {
    /* stub */
} OXLPalSayBubble;

    /*
typedef void (*oxl_client_conn_open_start_cb)(void *data);
typedef void (*oxl_client_conn_open_complete_cb)(void *data);
typedef void (*oxl_client_conn_failed_cb)(void *data);
typedef void (*oxl_client_conn_close_cb)(void *data);
typedef void (*oxl_client_goto_url_cb)(void *data);
typedef void (*oxl_client_room_changed_cb)(void *data);
typedef void (*oxl_client_auth_req_cb)(void *data);
typedef void (*oxl_client_security_err_cb)(void *data);
    */
/*
typedef void (*oxl_client_logon_cb)(void *data);
typedef void (*oxl_client_gotoroom_cb)(void *data);
typedef void (*oxl_client_say_cb)(void *data);
*/

/*
typedef struct oxl_callbacks_s {
    oxl_client_conn_open_start_cb *client_conn_start_cb;
    oxl_client_conn_open_complete_cb *client_conn_complete_cb;
    oxl_client_conn_failed_cb *client_conn_failed_cb;
    oxl_client_conn_close_cb *client_conn_close_cb;
    oxl_client_goto_url_cb *client_goto_url_cb;
    oxl_client_room_changed_cb *client_room_changed_cb;
    oxl_client_auth_req_cb *client_auth_req_cb;
    oxl_client_security_err_cb *client_security_err_cb;
*/
    /*
    oxl_client_logon_cb *client_logon_cb;
    oxl_client_goto_room_cb *client_goto_room_cb;
    oxl_client_say_cb *client_say_cb;
    */
/*
} oxl_callbacks_t;
*/

typedef struct OXLPalCallbacksStruct {
    void (*FinishOpenPalConn)(void *data);
    void (*FinishOpenPalConnnSuccess)(void *data);
    void (*FinishOpenPalConnFail)(void *data);
    void (*FinishClosePalConn)(void *data);
    void (*FinishGotoPalURL)(void *data);
    void (*FinishPalRoomChanged)(void *data);
    void (*FinishPalAuthRequest)(void *data);
    void (*FinishPalSecurityError)(void *data);
} OXLPalCallbacks;

typedef struct OXLPalUserStruct {
    PAL_HEADER_FIELDS
    int id;
    char name[32];
    int x;
    int y;
    int roomID;
    char roomName[256];
    int propCount;
    int face;
    int color;
    int flags;
    /* int prop_ids[256]; */
    /* int prop_crcs[256]; */
    /* oxl_client_prop_t props[256]; */
    void *propIDs;
    void *propCRCs;
    void *props;
    int showFaceFlag;
    OXLPalPropStore propStore;
} OXLPalUser;

typedef struct OXLPalViewStruct {
    /* stub */
} OXLPalView;

typedef struct OXLPalHotspotStruct {
    /* stub */
} OXLPalHotSpot;

typedef struct OXLPalRoomStruct {
    int id;
    char *name;
    char *backgroundFilename;
    OXLPalUser *users;
    void *usersHash;
    int roomFlags;
    void *spotImages;
    OXLPalHotSpot *hotspots;
    OXLPalHotSpot *hotspotsAboveNothing;
    OXLPalHotSpot *hotspotsAboveAvatars;
    OXLPalHotSpot *hotspotsAboutNametags;
    OXLPalHotSpot *hotspotsAboveEverything;
    void *hotspotsByID;
    OXLPalProp *looseProps;
    void *drawFrontCommands;
    void *drawBackCommands;
    void *drawLayerHistory;
    OXLPalUser *selectedUser;
    int userID;
    OXLPalView *roomView;
    double dimLevel;
    int showAvatarsFlag;
    char *chatLog;
    char *lastMessage;
    int lastMessageCount;
    int lastMessageReceived;
    int lastMessageTimer;
    char *statusMessage;
    int statusDisappearTimer;
} OXLPalRoom;

/*enum PalConnectionStateEnum { */
/*    c_busy, */ /* Busy; waiting for incoming data or for a write to complete. */
/*    c_done, */ /* Done; read incoming data or write finished. */
/*    c_stop, */ /* Stopped. */
/*    c_dead */
/*} PalConnectionState; */
    
typedef enum OXLPalClientStateEnum {
    STATE_DISCONNECTED,
    STATE_HANDSHAKING,
    STATE_CONNECTED /* STATE_READY */
} OXLPalClientState;

typedef struct PalClientStruct {
    PAL_HEADER_FIELDS
    OXLPalCrypt crypt;
    uv_tcp_t *conn;
    uv_loop_t *loop;
    /* uv_getaddrinfo_t resolver; */
    struct addrinfo *ai;
    int sockfd;
    OXLPalClientState state;
    char *host;
    uint port;
    uint roomID;
    char servername[PAL_SERVER_NAME_SZ_CAP];
    char username[PAL_USERNAME_SZ_CAP];
    char wizpass[PAL_WIZ_PASS_SZ_CAP];
    int PUIDChangedFlag;
    uint PUIDCounter;
    uint PUIDCRC;
    uint regCounter;
    uint regCRC;
    int serverIsBigEndianFlag;
    int waitingForMore;
    OXLPalRoom room;
    int serverVersion;
    int permissions;
    OXLPalPropStore propStore;
    OXLPalUser user;
    OXLPalCallbacks callback;
} OXLPalClient;

/* Regcode Section */
typedef struct PalRegcodeStruct {
    uint32_t counter;
    uint32_t crc;
} PalRegcode;

void StringToPalRegcode(const char *str, PalRegcode *regcode);
void GeneratePalRegcode(PalRegcode *regcode);
void PrintPalRegcode(const PalRegcode *regcode);
void TrimPalRegcodeString(char *trimmed_regcode_str, const char *str);
int32_t TrimLenOfPalRegcode(const char *str);

void InitPalCrypt(OXLPalCrypt *crypt);
void PalEncrypt(const OXLPalCrypt crypt, const char pt, char *ct);
void PalDecrypt(const OXLPalCrypt crypt, const char ct, char *pt);

typedef struct OXLPalSayCmdStruct {
    PAL_HEADER_FIELDS
    ushort len;
    char msg[PAL_CRYPT_CIPHERTEXT_SZ_CAP];
} OXLPalSayCmd;

void OXLInitPalSayCmd(OXLPalSayCmd *sayCmd, OXLPalClient *client, char *txt);
void OXLDumpPalSayCmd(OXLPalSayCmd *sayCmd);

/* typedef struct __attribute__((packed)) oxl_net_logon_s { */
typedef struct OXLPalLogonCmdStruct {
    PAL_HEADER_FIELDS
    uint regCRC;
    uint regCounter;
    byte usernameLen;
    char username[PAL_USERNAME_SZ_CAP];
    char wizpass[PAL_WIZ_PASS_SZ_CAP];
    uint flags;
    uint PUIDCounter;
    uint PUIDCRC;
    uint demoElapsed;
    uint totalElapsed;
    uint demoLimit;
    short initialRoomID;
    char reserved[PAL_RESERVED_LOGON_LEN];
    uint ulUploadRequestedProtocolVersion;
    uint ulUploadCapabilities;
    uint ulDownloadCapabilities;
    uint ul2DEngineCapabilities;
    uint ul2DGraphicsCapabilities;
    uint ul3DEngineCapabilities;
} OXLPalLogonCmd;

typedef struct OXLPalServerInfoMsgStruct {
    PAL_HEADER_FIELDS
    int permissions;
    unsigned char size;
    char serverName[PAL_SERVER_NAME_SZ_CAP];
} OXLPalServerInfoMsg;

void OXLInitPalLogonCmd(OXLPalLogonCmd *logonCmd,
                         char *username,
                         char *wizpass,
                         int initialRoomID,
                         uint regCRC,
                         int regCounter,
                         uint PUIDCRC,
                         int PUIDCounter);
    
void OXLDumpPalLogonCmd(const OXLPalLogonCmd *logonCmd);

/* init client */
/* void oxl_client_init(oxl_client_t *self, uv_loop_t *loop); */
/* start client */
void OXLOpenPalConnection(OXLPalClient *client,
                           uv_loop_t *loop,
                           char *username,
                           char *wizpass,
                           char *host,
                           uint port,
                           uint initialRoom);

void OXLClosePalConnection(OXLPalClient *client);

void OXLPalSay(OXLPalClient *client, size_t size, uv_buf_t buf, uv_write_cb finishSay);

/* Room Section */
typedef struct OXLRoomMsgStruct {
    PAL_HEADER_FIELDS
    int roomFlags;
    int face;
    short roomID;
    short roomNameOffset;
    short imageNameOffset;
    short artistNameOffset;
    short passwordOffset;
    short hotSpotCount;
    short hotSpotOffset;
    short imageCount;
    short imageOffset;
    short drawCommandsCount;
    short firstDrawCommandOffset;
    short peopleCount;
    short loosePropCount;
    short firstLoosePropOffset;
    short reserved;
    short dataLen;
    unsigned char *data;
} OXLRoomMsg;

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
    int refID;
    short code;
} OXLPalNavErr;

void OXLInitPalNavErr(OXLPalNavErr *navErr, const byte *data, const uint refID);

void OXLPalRoomMsgToPalRoom(OXLPalRoom *room, const byte *data, const uint refID);

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

void OXLInitPalRoom(OXLPalRoom *room);
void OXLFinishSelectedPalUserChanged(OXLPalUser *user);

typedef struct OXLGotoPalRoomCmdStruct {
    PAL_HEADER_FIELDS
    short roomID;
} OXLGotoPalRoomCmd;

void OXLInitGotoPalRoomCmd(OXLGotoPalRoomCmd *room, uint userID, short gotoRoomID);
void OXLLeavePalRoomCmd(OXLPalClient *client, OXLPalRoom *room);
void OXLJoinPalRoomCmd(OXLPalClient *client, uint gotoRoomID, void *finishJoinPalRoom);
/* void oxl_client_goto_room(oxl_client_t *self, uint goto_room_id); */

/* don't export */
#undef PAL_HEADER_FIELDS

#ifdef __cplusplus
}
#endif
#endif /* OXYALE_H_ */
