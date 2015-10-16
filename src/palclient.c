//
//  palclient.c
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <libuv/uv.h>
#include <oxlcom.h>
#include <palcom.h>
#include <palclient.h>
#include <pallogon.h>
#include <palserverinfo.h>
#include <palgotoroom.h>
#include <palsaybubble.h>
#include <palsay.h>
#include <palping.h>
#include <paljoinroom.h>
#include <palleaveroom.h>

/*
#include <stdlib.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <uv.h>
#include <oxyale.h>
#include <oxyale-palclient.h>
*/

/**************************************************************
 * Private static with static deps and callbacks **************
 **************************************************************/

/* uv_write_cb */
void
OXLPalClientAfterSendAndDestroyMsg_(uv_write_t *write, const int32_t status)
{
    OXLWriteBaton *wb = (OXLWriteBaton *)write->data;
    if (wb->cb.callback) {
        wb->cb.callback(wb->cb.sender, wb->cb.data, status);
    }
    OXLDestroyWriteBaton(wb);
}

/* uv_write_cb */
void
OXLPalClientAfterSendAndRetainMsg_(uv_write_t *write, const int32_t status)
{
    OXLWriteBaton *wb = (OXLWriteBaton *)write->data;
    if (wb->cb.callback) {
        wb->cb.callback(wb->cb.sender, wb->cb.data, status);
    }
    OXLFree(write);
}

static OXLWriteBaton *
OXLPalClientSendMsgInit_(OXLPalClient *client, const void *msg, const OXLCallback callback)
{
    const OXLPalMsg *palMsg = msg;
    
    OXLDumpPalMsg(palMsg);
    
    uint32_t msgSize = sizeof(OXLPalMsg) + palMsg->len;
    OXLWriteBaton *wb = malloc(sizeof(*wb));
    
    OXLCallbackBaton cbInit = { .sender = client, .data = msg, .callback = callback };
    memcpy((OXLCallback *)&wb->cb, &cbInit, sizeof(wb->cb));
    
    /* wb->buf = uv_buf_init((char *)msg, msgSize); */
    uv_buf_t buf = uv_buf_init((char *)msg, msgSize);
    memcpy((uv_buf_t *)&wb->buf, &buf, sizeof(buf));
    
    wb->write.data = wb;
    /*
     wb->cb.sender = client;
     wb->cb.data = msg;
     wb->cb.callback = callback;
     */
    return wb;
}

static void
OXLPalClientSendAndDestroyMsg_(OXLPalClient *client, void *msg, const OXLCallback callback)
{
    OXLWriteBaton *wb = OXLPalClientSendMsgInit_(client, msg, callback);
    uv_write(&wb->write, (uv_stream_t *)client->conn, &wb->buf, 1, OXLPalClientAfterSendAndDestroyMsg_);
}

static void
OXLPalClientSendAndRetainMsg_(OXLPalClient *client, const void *msg, const OXLCallback callback)
{
    OXLWriteBaton *wb = OXLPalClientSendMsgInit_(client, msg, callback);
    uv_write(&wb->write, (uv_stream_t *)client->conn, &wb->buf, 1, OXLPalClientAfterSendAndRetainMsg_);
}

/*
 * status codes:
 * 0<: logon success with warning
 * 0: logon success
 * <0: logon fail with error
 */
void
OXLPalClientAfterLogon_(void *sender, const void *data, const int32_t status)
{
    OXLLog("OXLPalClientAfterLogon");
    OXLPalClient *client = (OXLPalClient *)sender;
    OXLWriteBaton *wb = (OXLWriteBaton *)data;
    
    if (status < 0) {
        OXLLog("ERROR. Logon write failure. STATE_DISCONNECTED is now true.");
        client->state = STATE_DISCONNECTED;
        /* uv_close((uv_handle_t *)client->conn, NULL); */
        OXLDestroyWriteBaton(wb);
    } else if (0 <= status) {
        OXLLog("Logon write successful. STATE_CONNECTED is now true.");
        client->state = STATE_CONNECTED;
        OXLDestroyWriteBaton(wb);
    }
    client->event.PalLogonEvent(client, NULL, status);
}

/* static */ void
OXLPalClientPerformLogon_(OXLPalClient *client, const uint32_t size, const uint32_t refId)
{
    OXLLog("OXLPalaceLogon(client: 0x%x, size: 0x%x, refId: 0x%x)", (uint32_t)client, size, refId);
    client->currentRoom.userId = refId;
    
    OXLPalLogonMsg *logonMsg = OXLCreatePalLogonMsg(client->username,
                                                    client->wizpass,
                                                    client->roomId,
                                                    client->regCRC,
                                                    client->regCounter,
                                                    client->puidCRC,
                                                    client->puidCounter);
    
    OXLDumpPalLogonMsg(logonMsg);
    OXLDumpRawBufWithSize((byte *)logonMsg, sizeof(*logonMsg));
    
    OXLPalClientSendAndDestroyMsg_((OXLPalClient *)client, logonMsg, OXLPalClientAfterLogon_);
}

/***********************************************
 * Private Static (inline preferred) ***********
 ***********************************************/

static void
OXLPalClientHandleAltLogon_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef, const uv_buf_t *buf)
{
    OXLPalLogonMsg *altLogonMsg = (OXLPalLogonMsg *)buf->base;
    if (altLogonMsg->puidCounter != client->puidCounter || altLogonMsg->puidCRC != client->puidCRC) {
        OXLLog("PUID changed by server.");
        client->puidCRC = altLogonMsg->puidCRC;
        client->puidCounter = altLogonMsg->puidCounter;
        client->puidChangedFlag = 1;
    }
    
    /* emit alt_logon received event signal */
    /* client->event.PalEventAltLogon(client); */
}

/*
static void
OXLPalClientHandleAuthenticate_(OXLPalClient *client, OXLCallback callback)
{
    callback(client, NULL, 1);
}
 */

static void
OXLPalClientHandleConnErr_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef)
{
    OXLLog("HandleConnErr");
    switch (msgRef) {
        case 0x04:
        case 0x07:
            OXLLog("You have been killed.");
            break;
        case 0x0d:
            OXLLog("You have been kicked off the site.");
            break;
        case 0x0b:
            OXLLog("Your death penalty is still active.");
            break;
        case 0x0c:
            OXLLog("You are not currently allowed on this site.");
            break;
        case 0x06:
            OXLLog("Your connection was terminated due to inactivity");
            break;
        case 0x03:
            OXLLog("Your connection was terminated due to flooding.");
            break;
        case 0x08:
            OXLLog("This palace is currently full. Please try again later.");
            break;
        case 0x0e:
            OXLLog("Guests are not currently allowed on this site.");
            break;
        case 0x05:
            OXLLog("This palace was shut down by its operator. Try again later.");
            break;
        case 0x09:
            OXLLog("You have an invalid serial number.");
            break;
        case 0x0a:
            OXLLog("There is another user using your serial number.");
            break;
        case 0x0f:
            OXLLog("Your free demo has expired.");
            break;
        case 0x10:
            OXLLog("Unknown error?");
            break;
        case 0x02:
            OXLLog("There has been a communications error.");
            break;
        default:
            OXLLog("ERROR Unknown [PUIDChangedFlag: 0x%x, msgLen: 0x%x, msgRef: 0x%x]",
                   client->puidChangedFlag, msgLen, msgRef);
            break;
    }
    
    if (!client->puidChangedFlag) {
        /* PUID unchanged. */
        OXLLog("Connection dropped.");
    }
    
    /* emit connection error received event signal */
    client->event.PalConnectEvent(client, NULL, -1);
}

static void
OXLPalClientHandlePong_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef)
{
    client->event.PalPongEvent(client, NULL, 0);
}

static void
OXLPalClientHandleServerInfo_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef, const uv_buf_t *buf)
{
    OXLPalServerInfoMsg *serverInfo = (OXLPalServerInfoMsg *)buf;
    client->permissions = serverInfo->permissions;
    strlcpy(client->servername, serverInfo->servername, PAL_SERVER_NAME_SZ_CAP);
    
    /* TODO emit handle server info received event signal  */
    client->event.PalServerInfoEvent(client, NULL, 0);
}

static void
OXLPalClientHandleServerVersion_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef, const uv_buf_t *buf)
{
    OXLLog("HandleServerVersion");
    
    client->serverVersion = msgRef;
    OXLLog("Palace server ver %u", client->serverVersion);
    
    /* emit palace server version received event signal */
}

static void
OXLPalClientHandleRecvUserStatus_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef, const uv_buf_t *buf)
{
    OXLPalUser *user = (OXLPalUser *)buf->base;
    
    /* if (-1 < client->user.id) { */
    if (client->user.id) {
        client->user.flags = user->flags;
    }
    /* TODO emit handle user status received event signal  */
}

static void
OXLPalClientHandleMaxUsersLoggedOn_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef, const uv_buf_t *buf)
{
    /* TODO emit user logged on and max received event signal */
}

static void
OXLPalClientHandleXTalk_(OXLPalClient *client, const uint32_t msgLen, const uint32_t msgRef, const uv_buf_t *buf)
{
    char *cipherText = buf->base;
    char *plainText = OXLAlloc(strnlen(buf->base, OXL_MAX_STR_SZ_CAP)); /* buf->len? */
    OXLPalCryptoDecrypt(client->crypto, cipherText, plainText);
    OXLLog("plainText: %s", plainText);
    client->event.PalXTalkEvent(client, plainText, 0);
    OXLFree(plainText);
}

static void
OXLPalClientPerformHandshake_(OXLPalClient *client, const ssize_t nread, const uv_buf_t *buf)
{
    OXLLog("PerformHandshake");
    if (0 < nread) {
        OXLLog("%ld bytes read.", nread);
        OXLPalMsg *msg = (OXLPalMsg *)buf->base;
        uint32_t msgId = msg->id;
        
        /* endian test follows in switch statement */
        /* *(int *)buf->base; */
        uint32_t msgLen;
        uint32_t msgRef;
        
        switch (msgId) {
            case PAL_RX_UNKNOWN_SERVER:
                OXLLog("Received MSG_TROPSER during handshake. Execution stops here.");
                break;
            case PAL_RX_LITTLE_ENDIAN_SERVER: /* MSG_DIYIT */
                OXLLog("Server is little endian.");
                /* TODO will a server EVER send little endian data? */
                /* Big endian is standard for network communication. */
                client->serverIsBigEndianFlag = 0;
                /* TODO need to accomodate for endianness? */
                /* i.e. little endian server, big endian client. */
                msgLen = msg->len;
                msgRef = msg->ref;
                OXLPalClientPerformLogon_(client, msgLen, msgRef);
                break;
            case PAL_RX_BIG_ENDIAN_SERVER: /* MSG_TIYID */
                OXLLog("Server is big endian.");
                client->serverIsBigEndianFlag = 1;
                msgLen = msg->len; /* ntohl(header->msgLen); */
                msgRef = msg->ref; /* ntohl(header->msgRef); */
                OXLPalClientPerformLogon_(client, msgLen, msgRef);
                break;
            default:
                OXLLog("Unexpected error while logging in.");
                break;
        } /* end switch */
        /* msgID = req->data */
    } else {
        /* TODO possible error? no bytes read. */
    }
}

/***********************************************
 * Private *************************************
 ***********************************************/

void OXLPalClientAfterDisconnect_(uv_handle_t *handle)
{
    OXLPalClient *client = handle->data;
    client->state = STATE_DISCONNECTED;
}

/* uv_timer_cb */
void OXLPalClientTimerPingServer_(uv_timer_t *timer)
{
    OXLTimerBaton *tb = (OXLTimerBaton *)timer;
    OXLPalClient *client = (OXLPalClient *)timer->data;
    
    if (client->state != STATE_CONNECTED) {
        return;
    }
    
    OXLPalClientSendAndRetainMsg_(client, (void *)&gPalPingMsg, tb->cb.callback);
}

/* uv_read_cb */
void OXLPalClientAfterRead_(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    OXLPalMsg *msg;
    OXLPalClient *client = (OXLPalClient *)stream->data;
    
    OXLLog("OXLPalClientAfterRead");
    if (nread == 0) {
        OXLLog("Read ZERO bytes from server.");
    }
    else if (nread < 0) { /* error */
        OXLLog("Read %s bytes. ERROR reading data, terminating connection.", nread);
        /* uv_close((uv_handle_t *)stream, NULL); */
        OXLPalClientDisconnect(client);
    }
    else if (0 < nread) { /* else ? */
        /* we need to collect payloads then route the message (on a worker thread?) */
        /* we need to do this depending on the state of the connection */
        OXLLog("Successfully read %ld bytes of data", nread);
        
        OXLLog("Read the following:");
        OXLDumpBufWithSize(*buf, nread);
        
        if (client->state == STATE_HANDSHAKING) {
            OXLLog("STATE_HANDSHAKING is true. Performing handshake.");
            OXLPalClientPerformHandshake_(client, nread, buf);
        }
        else if (client->state == STATE_CONNECTED) {
            /* fprintf(stderr,
             "--- DEBUG: reading data with STATE_CONNECTED true.\n"); */
            
            if (client->msg.id == 0) {
                /* fprintf(stderr, "--- DEBUG: client->msgId == 0\n"); */
                if (PAL_NET_HEADER_LEN <= nread) {
                    msg = (OXLPalMsg *)buf->base;
                    /* *((int *)buf->base)); */
                    client->msg.id = msg->id;
                    /* *((int *)(buf->base + sizeof client->msgId))); */
                    client->msg.len = msg->len;
                    /* *((int *)(buf->base + */
                    /* sizeof client->msgId + sizeof client->msgLen))); */
                    client->msg.ref = msg->ref;
                    /* oxl_header_t *header = malloc(nread);
                     memcpy(header, buf->base, nread);
                     free(header); */ /* do something with it instead */
                }
                else {
                    return; /* TODO ? */
                }
            }
            
            if (nread < msg->len) {
                return; /* TODO ? */
            }
            
            int32_t msgId = msg->id; /* resolve indirection only once */
            int32_t msgLen = msg->len;
            int32_t msgRef = msg->ref;
            
            switch (msgId) {
                case PAL_RX_ALTERNATE_LOGON_REPLY:
                    OXLLog("Read message ALTERNATE_LOGON_REPLY.");
                    OXLPalClientHandleAltLogon_(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_CONNECTION_ERROR:
                    OXLLog("Read message CONNECTION_ERROR.");
                    OXLPalClientHandleConnErr_(client, msgLen, msgRef);
                    break;
                case PAL_RX_SERVER_VERSION:
                    OXLLog("Read message SERVER_VERSION.");
                    OXLPalClientHandleServerVersion_(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_SERVER_INFO:
                    OXLLog("DEBUG: Read message SERVER_INFO.");
                    OXLPalClientHandleServerInfo_(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_USER_STATUS:
                    OXLLog("Read message USER_STATUS.");
                    OXLPalClientHandleRecvUserStatus_(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_USER_LOGGED_ON_AND_MAX:
                    OXLLog("Read message USER_LOGGED_ON_AND_MAX.");
                    OXLPalClientHandleMaxUsersLoggedOn_(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_GOT_HTTP_SERVER_LOCATION:
                    OXLLog("Read message GOT_HTTP_SERVER_LOCATION.");
                    break;
                case PAL_RX_GOT_ROOM_DESCRIPTION:
                    OXLLog("Read message GOT_ROOM_DESCRIPTION.");
                    break;
                case PAL_RX_GOT_ROOM_DESCRIPTION_ALT:
                    OXLLog("Read message GOT_ROOM_DESCRIPTION_ALT.");
                    break;
                case PAL_RX_GOT_USER_LIST:
                    OXLLog("Read message GOT_USER_LIST.");
                    break;
                case PAL_RX_GOT_REPLY_OF_ALL_USERS:
                    OXLLog("Read message GOT_REPLY_OF_ALL_USERS.");
                    break;
                case PAL_RX_GOT_ROOM_LIST:
                    OXLLog("Read message GOT_ROOM_LIST.");
                    break;
                case PAL_RX_ROOM_DESCEND:
                    OXLLog("Read message ROOM_DESCEND.");
                    break;
                case PAL_RX_USER_NEW:
                    OXLLog("Read message USER_NEW.");
                    break;
                case PAL_RX_PING:
                    OXLLog("Read message PONG.");
                    OXLPalClientHandlePong_(client, msgLen, msgRef);
                    break;
                case PAL_RX_XTALK:
                    OXLLog("Read message XTALK.");
                    OXLPalClientHandleXTalk_(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_XWHISPER:
                    OXLLog("Read message XWHISPER.");
                    break;
                case PAL_RX_TALK:
                    OXLLog("Read message TALK.");
                    break;
                case PAL_RX_WHISPER:
                    OXLLog("Read message WHISPER.");
                    break;
                case PAL_RX_ASSET_INCOMING:
                    OXLLog("Read message ASSET_INCOMING.");
                    break;
                case PAL_RX_ASSET_QUERY:
                    OXLLog("Read message ASSET_QUERY.");
                    break;
                case PAL_RX_MOVEMENT:
                    OXLLog("Read message MOVEMENT.");
                    break;
                case PAL_RX_USER_COLOR:
                    OXLLog("Read message USER_COLOR.");
                    break;
                case PAL_RX_USER_FACE:
                    OXLLog("Read message USER_FACE.");
                    break;
                case PAL_RX_USER_PROP:
                    OXLLog("Read message USER_PROP.");
                    break;
                case PAL_RX_USER_DESCRIPTION:
                    OXLLog("Read message USER_DESCRIPTION.");
                    break;
                case PAL_RX_USER_RENAME:
                    OXLLog("Read message USER_RENAME.");
                    break;
                case PAL_RX_USER_LEAVING:
                    OXLLog("Read message USER_LEAVING.");
                    break;
                case PAL_RX_USER_EXIT_ROOM:
                    OXLLog("USER_EXIT_ROOM.");
                    break;
                case PAL_RX_PROP_MOVE:
                    OXLLog("Read message PROP_MOVE.");
                    break;
                case PAL_RX_PROP_DELETE:
                    OXLLog("DEBUG: Read message PROP_DELETE.");
                    break;
                case PAL_RX_PROP_NEW:
                    OXLLog("Read message PROP_NEW.");
                    break;
                case PAL_RX_DOOR_LOCK:
                    OXLLog("Read message DOOR_LOCK.");
                    break;
                case PAL_RX_DOOR_UNLOCK:
                    OXLLog("Read message DOOR_UNLOCK.");
                    break;
                case PAL_RX_PICT_MOVE:
                    OXLLog("Read message PICT_MOVE.");
                    break;
                case PAL_RX_SPOT_STATE:
                    OXLLog("Read message SPOT_STATE.");
                    break;
                case PAL_RX_SPOT_MOVE:
                    OXLLog("Read message SPOT_MOVE.");
                    break;
                case PAL_RX_DRAW_MSG:
                    OXLLog("Read message DRAW_MSG.");
                    break;
                case PAL_RX_INCOMING_FILE:
                    OXLLog("Read message INCOMING_FILE.");
                    break;
                case PAL_RX_NAV_ERROR:
                    OXLLog("Read message NAV_ERROR.");
                    /* OXLPalClientHandleNavError(client, msgLen, msgRef, buf); */
                    break;
                case PAL_RX_AUTHENTICATE:
                    OXLLog("Read message AUTHENTICATE.");
                    break;
                case PAL_RX_BLOWTHRU:
                    OXLLog("Read message BLOWTHRU.");
                    break;
                default:
                    OXLLog("Received unmatched message: "
                           "[msgId = 0x%x, "
                           "msgLen = 0x%x, "
                           "msgRef = 0x%x]\n",
                           msgId,
                           msgLen,
                           msgRef);
                    break;
            }
            client->msg.id = 0;
        }
        else if (client->state == STATE_DISCONNECTED)
        {
            OXLLog("STATE_DISCONNECTED. Ignoring data.");
        }
    }
    OXLLog("OXLPalClientAfterRead: Freeing buf->base");
    if (buf->base) {
        OXLFree(buf->base);
    }
}

/* uv_connect_cb */
void OXLPalClientAfterConnect_(uv_connect_t *conn, const int32_t status)
{
    OXLLog("OXLPalClientAfterConnect");
    OXLPalClient *client = (OXLPalClient *)conn->data;
    
    if (status) { /* error condition */
        OXLLog("Error: %d. %s\n", status, uv_strerror(status));
        OXLPalClientDisconnect(client);
    } else {
        OXLLog("Connected successfully to %s", client->servername);
        client->state = STATE_HANDSHAKING;
        conn->handle->data = client;
        uv_read_start(conn->handle, OXLAllocBuf, OXLPalClientAfterRead_);
    }
    client->event.PalConnectEvent(client, NULL, status);
}

/* OXLCallback */
void OXLPalAfterLogon_(void *sender, const void *data, const int32_t status)
{
    OXLPalClient *client = (OXLPalClient *)sender;
    OXLPalLogonMsg *logonMsg = (OXLPalLogonMsg *)data;
    
    client->usernameLen = logonMsg->usernameLen;
    strncpy(client->username, logonMsg->username, PAL_USERNAME_SZ_CAP); /* PString */
    strlcpy(client->wizpass, logonMsg->wizpass, PAL_WIZPASS_SZ_CAP); /* CString */
    client->roomId = logonMsg->initialRoomId;
    
    OXLDestroyPalLogonMsg(logonMsg);
}

/* getaddrinfo_cb */
void OXLPalClientAfterResolve_(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    OXLLog("ResolveCallback status: %d", status);
    OXLPalClient *client = (OXLPalClient *)req->data;
    
    if (status < 0) {
        OXLLog("Failed to resolve host: %s", uv_strerror(status));
    } else if (res != NULL && res->ai_addr != NULL) {
        char addr[IP4_ADDR_SZ_CAP] = {'\0'};
        OXLLog("Before uv_ip4_name");
        
        /* need to appropraitely handle failure here. */
        /* currently, this just segfaults if uv_ip4_name failes. */
        uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, res->ai_addrlen);
        OXLLog("Resolved to: %s", addr);
        
        uv_connect_t *conn_req = malloc(sizeof *conn_req);
        uv_tcp_t *conn = malloc(sizeof *conn);
        
        uv_tcp_init(client->loop, conn);
        client->conn = conn;
        conn_req->data = client;
        uv_tcp_connect(conn_req, conn, (struct sockaddr *)res->ai_addr, OXLPalClientAfterConnect_);
    }
    uv_freeaddrinfo(res);
}

void OXLClientPalLogon_(OXLPalClient *client,
                        const char *username,
                        const char *wizpass,
                        const int16_t roomId)
{
    OXLPalLogonMsg *logonMsg = OXLCreatePalLogonMsg(username,
                                                    wizpass,
                                                    roomId,
                                                    client->regCRC,
                                                    client->regCounter,
                                                    client->puidCRC,
                                                    client->puidCounter);
    
    OXLLog("Before DumpPalLogonMsg");
    OXLDumpPalLogonMsg(logonMsg);
    OXLLog("After DumpPalLogonMsg");
    
    OXLPalClientSendAndDestroyMsg_(client, logonMsg, OXLPalClientAfterLogon_);
}

/***********************************************
 * Public **************************************
 ***********************************************/

void OXLPalClientDisconnect(OXLPalClient *client)
{
    OXLLog("Disconnecting from Palace %s", client->servername);
    /* OXLPalClient *client = handle->data; */
    /* oxl_client_t *client = (oxl_client_t *)handle->data; */
    client->conn->data = client;
    uv_close((uv_handle_t *)client->conn, OXLPalClientAfterDisconnect_);
}

void OXLInitPalClient(OXLPalClient *client)
{
    /* client->crypto = *OXLPalCryptoCreate(); */

    client->loop = uv_default_loop();
    uv_timer_init(client->loop, &client->txTimer);
    uv_timer_start(&client->txTimer, OXLPalClientTimerPingServer_, 100 * 1000, 100 * 1000);
    
    client->puidChangedFlag = 0;
    client->puidCounter = 0xf5dc385e;
    client->puidCRC = 0xc144c580;
    client->regCounter = 0xcf07309c;
    client->regCRC = 0x5905f923;
    
    client->usernameLen = 0;
    memset(&client->username, '\0', PAL_USERNAME_SZ_CAP);
    client->wizpassLen = 0;
    memset(&client->wizpass, '\0', PAL_WIZPASS_SZ_CAP);
    
    client->event.PalAuthRequestEvent = NULL;
    client->event.PalConnectEvent = NULL;
    client->event.PalDisconnectEvent = NULL;
    client->event.PalGotoURLEvent = NULL;
    client->event.PalLogonEvent = NULL;
    client->event.PalPingEvent = NULL;
    client->event.PalPongEvent = NULL;
    client->event.PalRoomChangeEvent = NULL;
    client->event.PalSecurityEvent = NULL;
    client->event.PalServerInfoEvent = NULL;
    client->event.PalXTalkEvent = NULL;
}

OXLPalClient *OXLCreatePalClient(void)
{
    OXLPalClient *client = malloc(sizeof(*client));
    OXLInitPalClient(client);
    return client;
}

void OXLDestroyPalClient(OXLPalClient *client)
{
    /* OXLPalCryptoDestroy(&client->crypto); */
    OXLPalClientDisconnect(client);
    /* uv_loop_close(client->loop); */
    /* free(client->loop); */
    free(client);
}

void OXLSetPalUsername(OXLPalClient *client, const char *username)
{
    strlcpy(client->username, username, PAL_USERNAME_SZ_CAP);
}

void OXLPalClientSendAndRetainMsg(OXLPalClient *client, const void *msg, const OXLCallback callback)
{
    OXLPalClientSendAndRetainMsg_(client, msg, callback);
}

void OXLPalClientSendAndDestroyMsg(OXLPalClient *client, void *msg, const OXLCallback callback)
{
    OXLPalClientSendAndDestroyMsg_(client, msg, callback);
}

void OXLPalClientConnect(OXLPalClient *client,
                         const char *username,
                         const char *wizpass,
                         const char *hostname,
                         const uint16_t port,
                         const uint16_t initialRoomId)
{
    /* size_t usernameLen = strlen(username) + 1; */
    /* size_t wizpassLen = strlen(wizpass) + 1; */
    
    /* client->username = calloc(z, PAL_MAX_USERNAME_LEN); */
    /* sizeof *client->username); */
    strncpy(client->username, username, PAL_USERNAME_SZ_CAP);
    strlcpy(client->wizpass, wizpass, PAL_WIZPASS_SZ_CAP);
    client->roomId = initialRoomId;
    
    char portString[IP_PORT_SZ_CAP];
    OXLInt2Str(port, portString, IP_PORT_SZ_CAP);
    if (port == 9998) {
        OXLLog("Connecting to palace://%s as %s", hostname, username);
    } else {
        OXLLog("Connecting to %s:%n as %s", hostname, portString, username);
    }
    
    struct addrinfo *hints = OXLAlloc(sizeof(*hints));
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;
    
    OXLLog("Before alloc uvgetaddrinfo");
    uv_getaddrinfo_t *resolver = OXLAlloc(sizeof(*resolver));
    resolver->data = client;
    
    OXLLog("client->loop = %x", client->loop);
    OXLLog("uv_default_loop() = %x", uv_default_loop());
    
    OXLLog("Before uv_getaddrinfo");
    int32_t r = uv_getaddrinfo(client->loop,
                               resolver,
                               OXLPalClientAfterResolve_,
                               hostname,
                               portString,
                               hints);
    
    OXLLog("After uv_getaddrinfo");
    if (r) {
        OXLLog("ERROR getaddrinfo call returned: %s", uv_strerror(r));
        return;
    }
}

void OXLPalClientSay(OXLPalClient *client, const char *plainText, const OXLCallback callback)
{
    if (client->state != STATE_CONNECTED || strnlen(plainText, OXL_MAX_STR_SZ_CAP) <= 0) {
        return;
    }
    
    OXLPalClientSendAndDestroyMsg_(client, OXLCreatePalSayMsg(client, plainText), callback);
}

void OXLPalClientJoinRoom(OXLPalClient *client, const uint16_t gotoRoomId, const OXLCallback callback)
{
    uint32_t userId = 0; /* stub */
    OXLPalClientSendAndDestroyMsg_(client, OXLCreatePalGotoRoomMsg(userId, gotoRoomId), callback);
}

void OXLPalClientLeaveRoom(OXLPalClient *client, const OXLPalRoom *room, const OXLCallback callback)
{
    /* Let all iptscrae ON LEAVE event handlers execute */
    /* TODO emit leave room event signal */
    callback(client, NULL, 0);
}

void OXLPalClientGotoRoom(OXLPalClient *client, uint16_t gotoRoomId, OXLCallback cbAfterLeave, OXLCallback cbAfterJoin)
{
    if (client->state != STATE_CONNECTED || client->roomId == gotoRoomId) {
        return;
    }
    
    /* OXLPalClientLeaveRoom(client, cbOnLeave); */
    OXLPalClientSendAndDestroyMsg_(client, OXLCreatePalLeaveRoomMsg(client), cbAfterLeave);
    /* OXLPalClientJoinRoom(client, gotoRoomId, cbOnJoin); */
    OXLPalClientSendAndDestroyMsg_(client, OXLCreatePalJoinRoomMsg(client, gotoRoomId), cbAfterJoin);
}
