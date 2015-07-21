//
//  oxyale-palclient.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_oxyale_palclient_h
#define oxyale_oxyale_palclient_h

#include <palcom.h>
#include <palcrypto.h>
#include <palroom.h>
#include <palevent.h>

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
    OXLPalCrypto crypto;
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
    int32_t puidChangedFlag;
    int32_t puidCounter;
    int32_t puidCRC;
    int32_t regCounter;
    int32_t regCRC;
    int32_t serverIsBigEndianFlag;
    int32_t waitingForMore;
    OXLPalRoom room;
    int32_t serverVersion;
    int32_t permissions;
    OXLPalPropStore propStore;
    OXLPalUser user;
    OXLPalEvent event;
} OXLPalClient;

/* init client */
/* void oxl_client_init(oxl_client_t *self, uv_loop_t *loop); */
/* start client */
void OXLPalClientOpenConnection(OXLPalClient *client,
                                uv_loop_t *loop,
                                char *username,
                                char *wizpass,
                                char *host,
                                uint16_t port,
                                int32_t initialRoom);

void OXLPalClientCloseConnection(OXLPalClient *client);

void OXLPalLeaveRoom(OXLPalClient *client, OXLPalRoom *room);
void OXLPalJoinRoom(OXLPalClient *client, int32_t gotoRoomID);

void OXLPalClientSay(OXLPalClient *client, size_t size, uv_buf_t buf, uv_write_cb finishSay);

#endif
