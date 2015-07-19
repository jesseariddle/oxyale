//
//  oxyale-palclient.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_oxyale_palclient_h
#define oxyale_oxyale_palclient_h

typedef struct OXLPalCryptStruct OXLPalCrypt;
typedef struct OXLPalRoomStruct OXLPalRoom;
typedef struct OXLPalCallbacksStruct OXLPalCallbacks;

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

typedef struct OXLPalClientStruct {
    uint32_t msgID;
    uint32_t msgLen;
    uint32_t msgRef;
    OXLPalCrypt crypt;
    uv_tcp_t *conn;
    uv_loop_t *loop;
    /* uv_getaddrinfo_t resolver; */
    struct addrinfo *ai;
    uint32_t sockfd;
    OXLPalClientState state;
    char *host;
    uint16_t port;
    uint32_t roomID;
    char servername[PAL_SERVER_NAME_SZ_CAP];
    char username[PAL_USERNAME_SZ_CAP];
    char wizpass[PAL_WIZ_PASS_SZ_CAP];
    int32_t PUIDChangedFlag;
    int32_t PUIDCounter;
    int32_t PUIDCRC;
    int32_t regCounter;
    int32_t regCRC;
    int32_t serverIsBigEndianFlag;
    int32_t waitingForMore;
    OXLPalRoom room;
    int32_t serverVersion;
    int32_t permissions;
    OXLPalPropStore propStore;
    OXLPalUser user;
    OXLPalCallbacks callbacks;
} OXLPalClient;

#endif
