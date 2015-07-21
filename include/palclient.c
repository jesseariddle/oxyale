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
#include <palcom.h>
#include <palclient.h>
#include <pallogon.h>
#include <palserverinfo.h>
#include <palgotoroom.h>
#include <palsaybubble.h>
#include <palsay.h>

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

void OXLPalClientFinishLogon(uv_write_t *req, int32_t status)
{
    fprintf(stderr, "--- DEBUG: oxl_client_do_logon_cb\n");
    OXLWriteReq *wr = (OXLWriteReq *)req;
    OXLPalClient *client = wr->req.data;
    
    if (status < 0) {
        fprintf(stderr, "--- DEBUG: ERROR. STATE_DISCONNECTED is now true.\n");
        client->state = STATE_DISCONNECTED;
        uv_close((uv_handle_t *)client->conn, NULL);
    }
    else if (0 <= status) {
        fprintf(stderr, "--- DEBUG: write successful. STATE_CONNECTED is now true.\n");
        client->state = STATE_CONNECTED;
    }
    
    if (wr) {
        if (wr->buf.base) {
            free(wr->buf.base);
        }
        free(wr);
    }

    if (client->state == STATE_CONNECTED) {
        client->event.PalEventLogonSuccess(client);
    }
    else {
        client->event.PalEventLogonFail(client);
    }
}

void OXLPalClientLogon(OXLPalClient *client, uint32_t size, uint32_t refID)
{
    fprintf(stderr, "--- DEBUG: OXLPalaceLogon(0x%x, 0x%x, 0x%x)\n", (uint32_t)client, size, refID);
    
    client->room.userID = refID;
    fprintf(stderr, "--- DEBUG: client->currentRoom.userID = 0x%x;\n", refID);
    
    OXLWriteReq *logonMsg = malloc(sizeof *logonMsg);
    fprintf(stderr, "--- DEBUG: allocated write_req for logonMsg\n");
    
    logonMsg->req.data = client;
    
    int32_t z = sizeof(OXLPalLogonCmd);
    fprintf(stderr, "--- DEBUG: sizeof(OXLPalaceLogonMsg) is %d\n", z);
    logonMsg->buf = uv_buf_init(malloc(z), z);
    fprintf(stderr, "--- DEBUG: uv_buf_init(malloc(%d), sizeof(%d));\n", z, z);
    
    int32_t i = 0;
    fprintf(stderr, "\n--- DEBUG: logon_wr->buf.base[%d]: %08x\n\n", i, logonMsg->buf.base[i]);
    
    OXLPalInitLogonCmd((OXLPalLogonCmd *)logonMsg->buf.base,
                       client->username,
                       client->wizpass,
                       client->roomID,
                       client->regCRC,
                       client->regCounter,
                       client->puidCRC,
                       client->puidCounter);
    
    fprintf(stderr, "--- DEBUG: before DumpPalLogonMsg\n");
    OXLPalDumpLogonCmd((OXLPalLogonCmd *)logonMsg->buf.base);
    fprintf(stderr, "--- DEBUG: after DumpPalLogonMsg\n");
    /*
     fprintf(stderr,
     "--- DEBUG: logon("
     "%s, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, &logonMsg);\n",
     client->username,
     client->room_id,
     client->reg_crc,
     client->reg_counter,
     client->puid_crc,
     client->puid_counter);
     */
    /* endianness? */
    /*
     fprintf(stderr,
     "--- DEBUG: uv_write(0x%x (write_req), 0x%x (client->conn), "
     "0x%x (&logonMsg), 1, 0x%x);\n",
     (uint32_t)&write_req,
     (uint32_t)client->conn,
     (uint32_t)&logonMsg,
     (uint32_t)&finishLogon);
     */
    fprintf(stderr, "--- DEBUG: we are about to write the following:\n");
    OXLDumpBuf(logonMsg->buf, z);
    
    /* DumpPalBufPtr((uv_buf_t *)logonMsg->buf, z); */
    
    fprintf(stderr, "--- DEBUG: calling uv_write\n");
    uv_write((uv_write_t *)logonMsg, (uv_stream_t *)client->conn, &logonMsg->buf, 1, OXLPalClientFinishLogon);
    /*
     write_data((uv_stream_t *)client->conn,
     z,
     logonMsg,
     finishLogon);
     */
}

void OXLPalClientPerformHandshake(OXLPalClient *client, ssize_t nread, const uv_buf_t *buf)
{
    fprintf(stderr, "--- DEBUG: handshake\n");
    if (0 < nread) {
        fprintf(stderr, "--- DEBUG: %ld bytes read\n", nread);
        OXLPalHeader *header = (OXLPalHeader *)buf->base;
        uint32_t msgID = header->msgID;
        
        /* endian test follows in switch statement */
        /* *(int *)buf->base; */
        uint32_t msgLen;
        uint32_t msgRef;
        
        switch (msgID) {
            case PAL_RX_UNKNOWN_SERVER:
                fprintf(stderr, "--- DEBUG: MSG_TROPSER. execution stops here.\n");
                break;
            case PAL_RX_LITTLE_ENDIAN_SERVER: /* MSG_DIYIT */
                fprintf(stderr, "--- DEBUG: Server is little endian.\n");
                /* TODO will a server EVER send little endian data? */
                /* Big endian is standard for network communication. */
                client->serverIsBigEndianFlag = 0;
                /* TODO need to accomodate for endianness? */
                /* i.e. little endian server, big endian client. */
                msgLen = header->msgLen;
                msgRef = header->msgRef;
                OXLPalClientLogon(client, msgLen, msgRef);
                break;
            case PAL_RX_BIG_ENDIAN_SERVER: /* MSG_TIYID */
                fprintf(stderr, "--- DEBUG: Server is big endian.\n");
                client->serverIsBigEndianFlag = 1;
                msgLen = header->msgLen; /* ntohl(header->msgLen); */
                msgRef = header->msgRef; /* ntohl(header->msgRef); */
                OXLPalClientLogon(client, msgLen, msgRef);
                break;
            default:
                fprintf(stderr, "--- DEBUG: unexpected error while logging in.\n");
                break;
        }
        /* msgID = req->data */
    }
    else {
        /* TODO possible error? no bytes read. */
    }
}

static void OXLPalClientHandleServerVersion(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef, const uv_buf_t *buf)
{
    fprintf(stderr, "--- DEBUG: handleServerVersion\n");
    
    client->serverVersion = msgRef;
    fprintf(stderr, "--- DEBUG: palace server version %u\n", client->serverVersion);
    
    /* emit palace server version received event signal */
}

static void OXLPalClientHandleConnErr(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef)
{
    fprintf(stderr, "--- DEBUG: handleConnErr\n");
    switch (msgRef) {
        case 0x04:
        case 0x07:
            fprintf(stderr, "--- DEBUG: You have been killed.\n");
            break;
        case 0x0d:
            fprintf(stderr, "--- DEBUG: You have been kicked off the site.\n");
            break;
        case 0x0b:
            fprintf(stderr, "--- DEBUG: Your death penalty is still active.\n");
            break;
        case 0x0c:
            fprintf(stderr, "--- DEBUG: You are not currently allowed "
                    "on this site.\n");
            break;
        case 0x06:
            fprintf(stderr, "--- DEBUG: Your connection was terminated "
                    "due to inactivity.\n");
            break;
        case 0x03:
            fprintf(stderr, "--- DEBUG: Your connection was terminated "
                    "due to flooding.\n");
            break;
        case 0x08:
            fprintf(stderr, "--- DEBUG: This palace is currently full. "
                    "Please try again later.\n");
            break;
        case 0x0e:
            fprintf(stderr, "--- DEBUG: Guests are not currently allowed "
                    "on this site.\n");
            break;
        case 0x05:
            fprintf(stderr, "--- DEBUG: This palace was shut down by "
                    "its operator. Try again later.\n");
            break;
        case 0x09:
            fprintf(stderr, "--- DEBUG: You have an invalid serial number.\n");
            break;
        case 0x0a:
            fprintf(stderr, "--- DEBUG: There is another user using "
                    "your serial number.\n");
            break;
        case 0x0f:
            fprintf(stderr, "--- DEBUG: Your free demo has expired.\n");
            break;
        case 0x10:
            fprintf(stderr, "--- DEBUG: Unknown error?\n");
            break;
        case 0x02:
            fprintf(stderr, "--- DEBUG: There has been a communications error.\n");
            break;
        default:
            fprintf(stderr,
                    "--- DEBUG: Unknown error "
                    "[PUIDChangedFlag: 0x%x, msgLen: 0x%x, msgRef: 0x%x]\n",
                    client->puidChangedFlag, msgLen, msgRef);
            break;
    }
    
    if (!client->puidChangedFlag) {
        fprintf(stderr, "--- DEBUG: puid unchanged. connection dropped.\n");
    }
    
    /* emit connection error received event signal */
    client->event.PalEventOpenConnFail(client);
}

void OXLPalClientHandleAltLogon(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef, const uv_buf_t *buf)
{
    OXLPalLogonCmd *altLogonCmd = (OXLPalLogonCmd *)buf->base;
    if (altLogonCmd->puidCounter != client->puidCounter ||
        altLogonCmd->puidCRC != client->puidCRC)
    {
        fprintf(stderr, "PUID changed by server.\n");
        client->puidCRC = altLogonCmd->puidCRC;
        client->puidCounter = altLogonCmd->puidCounter;
        client->puidChangedFlag = 1;
    }
    
    /* emit alt_logon received event signal */
    /* client->event.PalEventAltLogon(client); */
}

void OXLPalClientHandleServerInfo(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef, const uv_buf_t *buf)
{
    OXLPalServerInfoMsg *serverInfo = (OXLPalServerInfoMsg *)buf;
    client->permissions = serverInfo->permissions;
    strlcpy(client->servername, serverInfo->servername, PAL_SERVER_NAME_SZ_CAP);
    
    /* TODO emit handle server info received event signal  */
    client->event.PalEventServerInfo(client);
}

void OXLPalClientHandleUserStatus(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef, const uv_buf_t *buf)
{
    OXLPalUser *user = (OXLPalUser *)buf->base;
    if (-1 < client->user.id) {
        client->user.flags = user->flags;
    }
    /* TODO emit handle user status received event signal  */
}

void OXLPalClientHandleMaxUsersLoggedOn(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef, const uv_buf_t *buf)
{
    /* TODO emit user logged on and max received event signal */
}

void OXLPalClientFinishCloseConn(uv_handle_t *handle)
{
    fprintf(stderr, "--- DEBUG: oxl_client_do_conn_close_cb()\n");
    OXLPalClient *client = handle->data;
    /* oxl_client_t *client = (oxl_client_t *)handle->data; */
    /* client->state = STATE_DISCONNECTED; */
    /* close socket? */
    /* uv_close(handle->data) */
    free(client);
}

/* uv_read_cb */
void OXLPalClientFinishReadData(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    uint32_t msgID = 0;
    uint32_t msgLen;
    uint32_t msgRef;
    
    fprintf(stderr, "--- DEBUG: oxl_client_read_cb\n");
    if (nread == 0) {
        fprintf(stderr, "--- DEBUG: Read ZERO bytes.\n");
    }
    else if (nread < 0) { /* error */
        fprintf(stderr, "--- DEBUG: Error reading data, closing stream\n");
        uv_close((uv_handle_t *)stream, NULL);
    }
    else if (0 < nread) { /* else ? */
        /* we need to collect payloads then route the message (on a worker thread?) */
        /* we need to do this depending on the state of the connection */
        fprintf(stderr, "--- DEBUG: successfully read %ld bytes of data\n", nread);
        
        fprintf(stderr, "--- DEBUG: read the following:\n");
        fprintf(stderr, "--- DEBUG: ");
        OXLDumpBuf(*buf, nread);
        
        OXLPalClient *client = stream->data;
        if (client->state == STATE_HANDSHAKING) {
            fprintf(stderr, "--- DEBUG: STATE_HANDSHAKING is true. Performing handshake.\n");
            OXLPalClientPerformHandshake((OXLPalClient *)client, nread, buf);
        }
        else if (client->state == STATE_CONNECTED) {
            /* fprintf(stderr,
             "--- DEBUG: reading data with STATE_CONNECTED true.\n"); */
            
            if (client->msgID == 0) {
                /* fprintf(stderr, "--- DEBUG: client->msgID == 0\n"); */
                if (PAL_NET_HEADER_LEN <= nread) {
                    OXLPalHeader *header = (OXLPalHeader *)buf->base;
                    /* *((int *)buf->base)); */
                    client->msgID = header->msgID;
                    /* *((int *)(buf->base + sizeof client->msgID))); */
                    client->msgLen = header->msgLen;
                    /* *((int *)(buf->base + */
                    /* sizeof client->msgID + sizeof client->msgLen))); */
                    client->msgRef = header->msgRef;
                    /* oxl_header_t *header = malloc(nread);
                     memcpy(header, buf->base, nread);
                     free(header); */ /* do something with it instead */
                }
                else {
                    return; /* TODO ? */
                }
            }
            
            msgID = client->msgID;
            msgLen = client->msgLen;
            msgRef = client->msgRef;
            
            if (nread < msgLen) {
                return; /* TODO ? */
            }
            
            switch (msgID) {
                case PAL_RX_ALTERNATE_LOGON_REPLY:
                    fprintf(stderr, "--- DEBUG: Read message ALTERNATE_LOGON_REPLY.\n");
                    OXLPalClientHandleAltLogon(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_CONNECTION_ERROR:
                    fprintf(stderr, "--- DEBUG: Read message CONNECTION_ERROR.\n");
                    OXLPalClientHandleConnErr(client, msgLen, msgRef);
                    break;
                case PAL_RX_SERVER_VERSION:
                    fprintf(stderr, "--- DEBUG: Read message SERVER_VERSION.\n");
                    OXLPalClientHandleServerVersion(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_SERVER_INFO:
                    fprintf(stderr, "--- DEBUG: Read message SERVER_INFO.\n");
                    OXLPalClientHandleServerInfo(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_USER_STATUS:
                    fprintf(stderr, "--- DEBUG: Read message USER_STATUS.\n");
                    OXLPalClientHandleUserStatus(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_USER_LOGGED_ON_AND_MAX:
                    fprintf(stderr, "--- DEBUG: Read message USER_LOGGED_ON_AND_MAX.\n");
                    OXLPalClientHandleMaxUsersLoggedOn(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_GOT_HTTP_SERVER_LOCATION:
                    fprintf(stderr, "--- DEBUG: Read message GOT_HTTP_SERVER_LOCATION.\n");
                    break;
                case PAL_RX_GOT_ROOM_DESCRIPTION:
                    fprintf(stderr, "--- DEBUG: Read message GOT_ROOM_DESCRIPTION.\n");
                    break;
                case PAL_RX_GOT_ROOM_DESCRIPTION_ALT:
                    fprintf(stderr, "--- DEBUG: Read message GOT_ROOM_DESCRIPTION_ALT.\n");
                    break;
                case PAL_RX_GOT_USER_LIST:
                    fprintf(stderr, "--- DEBUG: Read message GOT_USER_LIST.\n");
                    break;
                case PAL_RX_GOT_REPLY_OF_ALL_USERS:
                    fprintf(stderr, "--- DEBUG: Read message GOT_REPLY_OF_ALL_USERS.\n");
                    break;
                case PAL_RX_GOT_ROOM_LIST:
                    fprintf(stderr, "--- DEBUG: Read message GOT_ROOM_LIST.\n");
                    break;
                case PAL_RX_ROOM_DESCEND:
                    fprintf(stderr, "--- DEBUG: Read message ROOM_DESCEND.\n");
                    break;
                case PAL_RX_USER_NEW:
                    fprintf(stderr, "--- DEBUG: Read message USER_NEW.\n");
                    break;
                case PAL_RX_PING:
                    fprintf(stderr, "--- DEBUG: Read message PING.\n");
                    break;
                case PAL_RX_XTALK:
                    fprintf(stderr, "--- DEBUG: Read message XTALK.\n");
                    break;
                case PAL_RX_XWHISPER:
                    fprintf(stderr, "--- DEBUG: Read message XWHISPER.\n");
                    break;
                case PAL_RX_TALK:
                    fprintf(stderr, "--- DEBUG: Read message TALK.\n");
                    break;
                case PAL_RX_WHISPER:
                    fprintf(stderr, "--- DEBUG: Read message WHISPER.\n");
                    break;
                case PAL_RX_ASSET_INCOMING:
                    fprintf(stderr, "--- DEBUG: Read message ASSET_INCOMING.\n");
                    break;
                case PAL_RX_ASSET_QUERY:
                    fprintf(stderr, "--- DEBUG: Read message ASSET_QUERY.\n");
                    break;
                case PAL_RX_MOVEMENT:
                    fprintf(stderr, "--- DEBUG: Read message MOVEMENT.\n");
                    break;
                case PAL_RX_USER_COLOR:
                    fprintf(stderr, "--- DEBUG: Read message USER_COLOR.\n");
                    break;
                case PAL_RX_USER_FACE:
                    fprintf(stderr, "--- DEBUG: Read message USER_FACE.\n");
                    break;
                case PAL_RX_USER_PROP:
                    fprintf(stderr, "--- DEBUG: Read message USER_PROP.\n");
                    break;
                case PAL_RX_USER_DESCRIPTION:
                    fprintf(stderr, "--- DEBUG: Read message USER_DESCRIPTION.\n");
                    break;
                case PAL_RX_USER_RENAME:
                    fprintf(stderr, "--- DEBUG: Read message USER_RENAME.\n");
                    break;
                case PAL_RX_USER_LEAVING:
                    fprintf(stderr, "--- DEBUG: Read message USER_LEAVING.\n");
                    break;
                case PAL_RX_USER_EXIT_ROOM:
                    fprintf(stderr, "--- DEBUG: Read message USER_EXIT_ROOM.\n");
                    break;
                case PAL_RX_PROP_MOVE:
                    fprintf(stderr, "--- DEBUG: Read message PROP_MOVE.\n");
                    break;
                case PAL_RX_PROP_DELETE:
                    fprintf(stderr, "--- DEBUG: Read message PROP_DELETE.\n");
                    break;
                case PAL_RX_PROP_NEW:
                    fprintf(stderr, "--- DEBUG: Read message PROP_NEW.\n");
                    break;
                case PAL_RX_DOOR_LOCK:
                    fprintf(stderr, "--- DEBUG: Read message DOOR_LOCK.\n");
                    break;
                case PAL_RX_DOOR_UNLOCK:
                    fprintf(stderr, "--- DEBUG: Read message DOOR_UNLOCK.\n");
                    break;
                case PAL_RX_PICT_MOVE:
                    fprintf(stderr, "--- DEBUG: Read message PICT_MOVE.\n");
                    break;
                case PAL_RX_SPOT_STATE:
                    fprintf(stderr, "--- DEBUG: Read message SPOT_STATE.\n");
                    break;
                case PAL_RX_SPOT_MOVE:
                    fprintf(stderr, "--- DEBUG: Read message SPOT_MOVE.\n");
                    break;
                case PAL_RX_DRAW_CMD:
                    fprintf(stderr, "--- DEBUG: Read message DRAW_CMD.\n");
                    break;
                case PAL_RX_INCOMING_FILE:
                    fprintf(stderr, "--- DEBUG: Read message INCOMING_FILE.\n");
                    break;
                case PAL_RX_NAV_ERROR:
                    fprintf(stderr, "--- DEBUG: Read message NAV_ERROR.\n");
                    break;
                case PAL_RX_AUTHENTICATE:
                    fprintf(stderr, "--- DEBUG: Read message AUTHENTICATE.\n");
                    break;
                case PAL_RX_BLOWTHRU:
                    fprintf(stderr, "--- DEBUG: Read message BLOWTHRU.\n");
                    break;
                default:
                    fprintf(stderr,
                            "--- DEBUG: received unmatched message: "
                            "[msgID = 0x%x, "
                            "msgLen = 0x%x, "
                            "msgRef = 0x%x]\n",
                            msgID,
                            msgLen,
                            msgRef);
                    break;
            }
            client->msgID = 0;
        }
        else if (client->state == STATE_DISCONNECTED)
        {
            /* disconnected, yeah? */
            fprintf(stderr,
                    "--- DEBUG: STATE_DISCONNECTED: freeing stream->data\n");
            free(stream->data);
            fprintf(stderr,
                    "--- DEBUG: STATE_DISCONNECTED: closing connection\n");
            uv_close((uv_handle_t *)stream, OXLPalClientFinishCloseConn);
        }
    }
    
    fprintf(stderr, "--- DEBUG: oxl_client_read_cb free(buf->base)\n");
    if (buf->base) {
        free(buf->base);
    }
}

/* uv_connect_cb */
void OXLPalClientFinishOpenConn(uv_connect_t *conn, int32_t status)
{
    fprintf(stderr, "--- DEBUG: oxl_client_conn_open_cb\n");
    OXLPalClient *client = conn->data;
    
    if (status) { /* error condition */
        fprintf(stderr,
                "--- DEBUG: error: %d. %s\n",
                status,
                uv_strerror(status));
        
        uv_close((uv_handle_t *)conn->handle, OXLPalClientFinishCloseConn);
    }
    else {
        fprintf(stderr, "--- DEBUG: connect successful.\n");
        client->state = STATE_HANDSHAKING;
        conn->handle->data = client;
        uv_read_start(conn->handle, OXLAllocBuf, OXLPalClientFinishReadData);
    }
}

/* getaddrinfo_cb */
static void OXLPalClientFinishResolvePalServer(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    fprintf(stderr, "--- DEBUG: resolvePalCallback\n");
    OXLPalClient *client = (OXLPalClient *)req->data;
    
    if (-1 == status) {
        fprintf(stderr,
                "getaddrinfo callback error %s\n",
                uv_strerror(status));
    }
    else {
        fprintf(stderr, "--- DEBUG: resolve status OK.\n");
        
        char addr[PAL_INET_ADDR_SZ_CAP] = {'\0'};
        fprintf(stderr, "--- DEBUG: before uv_ip4_name\n");
        
        /* need to appropraitely handle failure here. */
        /* currently, this just segfaults if uv_ip4_name failes. */
        uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, res->ai_addrlen);
        fprintf(stderr, "--- DEBUG: resolved to: %s\n", addr);
        
        uv_connect_t *conn_req = malloc(sizeof *conn_req);
        uv_tcp_t *conn = malloc(sizeof *conn);
        
        /* conn->data = client; */
        uv_tcp_init(client->loop, conn);
        client->conn = conn;
        conn_req->data = client;
        uv_tcp_connect(conn_req, conn, (struct sockaddr *)res->ai_addr, OXLPalClientFinishOpenConn);
    }
    uv_freeaddrinfo(res);
}

/* uv_write_cb */
void OXLPalClientFinishWrite(uv_write_t *req, int status)
{
    OXLWriteReq *wr = (OXLWriteReq *)req;
    
    if (0 <= status) {
        fprintf(stderr, "--- DEBUG: wrote: %s\n", req->data);
    }
    
    free(wr->buf.base);
    free(wr);
}

void OXLPalClientCloseConnection(OXLPalClient *client) {
    fprintf(stderr, "--- DEBUG: CloseConnection\n");
    /* free memory here */
    /* close connection? */
    /* free(client->host); */
}

void OXLPalClientOpenConnection(OXLPalClient *client,
                                uv_loop_t *loop,
                                char *username,
                                char *wizpass,
                                char *host,
                                uint16_t port,
                                int32_t initialRoomID)
{
    fprintf(stderr, "--- DEBUG: OpenConnection\n");
    
    /* init */
    OXLPalCryptoInit(&client->crypto);
    client->loop = loop;
    client->puidChangedFlag = 0;
    client->puidCounter = 0xf5dc385e;
    client->puidCRC = 0xc144c580;
    client->regCounter = 0xcf07309c;
    client->regCRC = 0x5905f923;
    
    /* start */
    size_t usernameLen = strlen(username) + 1;
    size_t wizpassLen = strlen(wizpass) + 1;
    
    /* client->username = calloc(z, PAL_MAX_USERNAME_LEN); */
    /* sizeof *client->username); */
    memset(&client->username, '\0', PAL_USERNAME_SZ_CAP);
    strlcpy(client->username, username, usernameLen);
    memset(&client->wizpass, '\0', PAL_WIZ_PASS_SZ_CAP);
    strlcpy(client->wizpass, wizpass, wizpassLen);
    client->roomID = initialRoomID;
    
    char portString[PAL_INET_PORT_SZ_CAP];
    snprintf(portString, PAL_INET_PORT_SZ_CAP - 1, "%d", port);
    fprintf(stderr, "--- DEBUG: host: %s\n", host);
    fprintf(stderr, "--- DEBUG: port: %s\n", portString);
    
    struct addrinfo *hints = malloc(sizeof *hints);
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;
    
    /* fprintf(stderr, "--- DEBUG: before uv_getaddrinfo\n"); */
    uv_getaddrinfo_t *resolver = malloc(sizeof *resolver);
    resolver->data = client;
    
    int32_t r = uv_getaddrinfo(loop, resolver, OXLPalClientFinishResolvePalServer, host, portString, hints);
    
    /* fprintf(stderr, "--- DEBUG: after uv_getaddrinfo\n"); */
    if (r) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_strerror(r));
        return;
    }
}

void OXLPalClientFinishJoinRoom()
{
    
    /* TODO emit room joined event signal */
}

void OXLPalClientJoinRoom(OXLPalClient *client, int32_t gotoRoomID)
{
    if (client->state != STATE_CONNECTED) {
        return;
    }
    
    OXLWriteReq *logonCmd = malloc(sizeof *logonCmd);
    fprintf(stderr, "--- DEBUG: allocated write_req\n");
    
    logonCmd->req.data = client;
    
    int32_t z = sizeof(OXLPalGotoRoomCmd);
    fprintf(stderr, "--- DEBUG: sizeof(OXL_GotoPalRoomCmd) is %d\n", z);
    logonCmd->buf = uv_buf_init(malloc(z), z);
    fprintf(stderr, "--- DEBUG: uv_buf_init(malloc(%d), sizeof(%d));\n", z, z);
    
    OXLPalGotoRoomCmdInit((OXLPalGotoRoomCmd *)logonCmd->buf.base, client->user.id, (short)gotoRoomID);
    
    client->room.selectedUser = NULL;
    
    fprintf(stderr, "--- DEBUG: we are about to write the following:\n");
    OXLDumpBuf(logonCmd->buf, z);
    
    /* oxl_dump_buf_p((uv_buf_t *)logon_wr->buf, z); */
    fprintf(stderr, "--- DEBUG: calling uv_write\n");
    uv_write((uv_write_t *)logonCmd,
             (uv_stream_t *)client->conn,
             &logonCmd->buf,
             1,
             OXLPalClientFinishJoinRoom);
}

void OXLPalClientLeaveRoom(OXLPalClient *client)
{
    /* Let all iptscrae ON LEAVE event handlers execute */
    /* TODO emit leave room event signal */
}

void OXLPalClientGotoRoom(OXLPalClient *client, int32_t gotoRoomID)
{
    if (client->state != STATE_CONNECTED || client->roomID == gotoRoomID) {
        return;
    }
    
    OXLPalClientLeaveRoom(client);
    OXLPalClientJoinRoom(client, gotoRoomID);
}

void OXLPalClientSay(OXLPalClient *client, size_t size, uv_buf_t buf, uv_write_cb finishWrite)
{
    fprintf(stderr, "--- DEBUG: OXLPalSay\n");
    fprintf(stderr, "--- DEBUG: received buf from app:\n");
    OXLDumpBuf(buf, size);
    
    /* OXLWriteReq *wr = OXLMakeWriteReq(client); */
    /* OXLSayBubble *bubble = OXLMakeSayBubble(client) */
    /* bubble->size = ((OXLSayBubbleCmd *)(buf.base))->size */
    /* OXLSayCmd *sayCmd = OXLMakeSayCmd(client) */
    
    OXLWriteReq *wr = malloc(sizeof *wr);
    wr->req.data = client;
    
    OXLPalSayBubble *bubble = malloc(sizeof *bubble);
    
    int32_t z = sizeof(OXLPalSayBubble);
    wr->buf = uv_buf_init(malloc(z), z);
    OXLPalSayCmdInit((OXLPalSayCmd *)wr->buf.base, client, buf.base);
    
    OXLPalSayCmdDump((OXLPalSayCmd *)wr->buf.base);
    
    fprintf(stderr, "--- DEBUG: preparing to send the following ciphertext:\n");
    OXLDumpBuf(wr->buf, size);
    //uv_write((uv_stream_t *)client->conn, size, wr->buf, finishWrite);
}

void OXLSetPalUsername(OXLPalClient *client, char *username) {
    strlcpy(client->username, username, PAL_USERNAME_SZ_CAP);
}
