//
//  oxyale-palclient.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palclient_h
#define oxyale_palclient_h

#include "oxlcom.h"
#include "palcom.h"
#include "palcrypto.h"
#include "palroom.h"
#include "palevent.h"
#include "palping.h"

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
    OXLPalMsg msg;
    OXLPalCrypto crypto;
    uv_tcp_t *socket;
    uv_loop_t *loop;
    uv_timer_t *txTimer;
    /* uv_getaddrinfo_t resolver; */
    struct addrinfo *ai;
    uint32_t sockfd;
    OXLPalClientState state;
    char hostname[OXL_MAX_STR_SZ_CAP];
    uint16_t port;
    uint16_t roomId;
    char servername[PAL_SERVER_NAME_SZ_CAP];
    int16_t usernameLen;
    char username[PAL_USERNAME_SZ_CAP];
    int16_t wizpassLen;
    char wizpass[PAL_WIZPASS_SZ_CAP];
    int32_t puidChangedFlag;
    int32_t puidCounter;
    int32_t puidCRC;
    int32_t regCounter;
    int32_t regCRC;
    int32_t serverIsBigEndianFlag;
    int32_t waitingForMore;
    OXLPalRoom currentRoom;
    int32_t serverVersion;
    int32_t permissions;
    uint32_t idlePingTimerStartDelayInSec;
    uint32_t idlePingTimerIntervalInSec;
    OXLPalPropStore propStore;
    OXLPalUser user;
    OXLPalEvent event;
} OXLPalClient;

OXLPalClient *OXLCreatePalClientWithLoop(uv_loop_t *uv_loop);
OXLPalClient *OXLCreatePalClient(void);
void OXLDestroyPalClient(OXLPalClient *client);
void OXLPalClientStartRead(OXLPalClient *client);
void OXLPalClientStopRead(OXLPalClient *client);

void OXLPalClientSetParams(OXLPalClient *client,
                           const char *username,
                           const char *wizpass,
                           const char *hostname,
                           const uint16_t port,
                           const uint16_t initialRoomId,
                           const uint32_t idlePingTimerStartDelayInSec,
                           const uint32_t idlePingTimerIntervalInSec);

void OXLPalClientConnectWithPingTimer(OXLPalClient *client,
                                      const char *username,
                                      const char *wizpass,
                                      const char *hostname,
                                      const uint16_t port,
                                      const uint16_t initialRoom,
                                      const uint32_t idlePingStartDelayInSec,
                                      const uint32_t idlePingIntervalInSec);

void OXLPalClientConnect(OXLPalClient *client,
                         const char *username,
                         const char *wizpass,
                         const char *hostname,
                         const uint16_t port,
                         const uint16_t initialRoom);

void OXLPalClientDisconnect(OXLPalClient *client);

void OXLPalClientLogon(OXLPalClient *client);
void OXLPalClientGotoRoom(OXLPalClient *client,
                          const uint16_t gotoRoomId,
                          const OXLCallback cbLeaveRoom,
                          const OXLCallback cbJoinRoom);

void OXLPalClientJoinRoom(OXLPalClient *client, const uint16_t gotoRoomId, const OXLCallback callback);
void OXLPalClientLeaveRoom(OXLPalClient *client, const OXLPalRoom *room, const OXLCallback callback);
void OXLPalClientSay(OXLPalClient *client, const char *plainText, const OXLCallback callback);
void OXLPalClientGetRoomList(OXLPalClient *client, const OXLCallback callback);

void OXLPalClientSendAndRetainMsg(OXLPalClient *client, const void *msg, const OXLCallback callback);
void OXLPalClientSendAndDestroyMsg(OXLPalClient *client, void *msg, const OXLCallback callback);

#endif
