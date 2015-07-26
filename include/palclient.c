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
    OXLLog("FinishLogon");
    OXLWriteReq *wr = (OXLWriteReq *)req;
    OXLPalClient *client = wr->req.data;
    
    if (status < 0) {
        OXLLog("ERROR. Logon write failure. STATE_DISCONNECTED is now true.");
        client->state = STATE_DISCONNECTED;
        /* uv_close((uv_handle_t *)client->conn, NULL); */
        OXLWriteReqDestroy(wr);
        client->event.PalEventLogonFail(client);
    }
    else if (0 <= status) {
        fprintf(stderr, "--- DEBUG: \n");
        OXLLog("Logon write successful. STATE_CONNECTED is now true.");
        client->state = STATE_CONNECTED;
        OXLWriteReqDestroy(wr);
        client->event.PalEventLogonSuccess(client);
    }
}

void OXLPalClientLogon(OXLPalClient *client, uint32_t size, uint32_t refID)
{
    OXLLog("OXLPalaceLogon(0x%x, 0x%x, 0x%x)", (uint32_t)client, size, refID);
    
    client->currentRoom.userID = refID;
    OXLLog("client->currentRoom.userID = 0x%x;", refID);
    
    OXLPalLogonCmd *logonCmd = OXLPalLogonCmdCreate(client->username,
                                                    client->wizpass,
                                                    client->roomID,
                                                    client->regCRC,
                                                    client->regCounter,
                                                    client->puidCRC,
                                                    client->puidCounter);
    OXLLog("Allocated logonCmd");
    
    OXLWriteReq *logonCmdWR = malloc(sizeof(*logonCmdWR));
    OXLLog("Allocated write_req for logonCmd");
    
    logonCmdWR->req.data = client;
    logonCmdWR->buf.base = (void *)logonCmd;
    logonCmdWR->buf.len = sizeof(*logonCmd);
    
    /* int32_t z = sizeof(OXLPalLogonCmd); */
    /* OXLLog("sizeof(OXLPalaceLogonMsg) is %d", z); */
    /* logonCmdWR->buf = uv_buf_init(malloc(z), z); */
    /* OXLLog("uv_buf_init(malloc(%d), sizeof(%d));", z, z); */
    
    /* int32_t i = 0;
    OXLLog("logonCmdWR->buf.base[%d]: %08x\n", i, logonCmdWR->buf.base[i]); */
    /*
    OXLPalLogonCmdInit((OXLPalLogonCmd *)logonCmdWR->buf.base,
                       client->username,
                       client->wizpass,
                       client->roomID,
                       client->regCRC,
                       client->regCounter,
                       client->puidCRC,
                       client->puidCounter);
    */
    
    OXLLog("Before LogonCmdDump");
    OXLPalLogonCmdDump((OXLPalLogonCmd *)logonCmdWR->buf.base);
    OXLLog("After LogonCmdDump");
    
    OXLBufDumpWithSize(logonCmdWR->buf, sizeof(OXLPalLogonCmd));
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
    
    OXLLog("Will write the following:");
    /* OXLBufDump(logonCmdWR->buf, z);*/
    OXLBufDumpWithSize(logonCmdWR->buf, sizeof(OXLPalLogonCmd));
    
    /* DumpPalBufPtr((uv_buf_t *)logonMsg->buf, z); */
    
    OXLLog("Calling uv_write");
    uv_stream_t *stream = (uv_stream_t *)client->conn;
    uv_write((uv_write_t *)logonCmdWR, stream, &logonCmdWR->buf, 1, OXLPalClientFinishLogon);
    /*
     write_data((uv_stream_t *)client->conn,
     z,
     logonMsg,
     finishLogon);
     */
}

void OXLPalClientPerformHandshake(OXLPalClient *client, ssize_t nread, const uv_buf_t *buf)
{
    OXLLog("PerformHandshake");
    if (0 < nread) {
        OXLLog("%ld bytes read.", nread);
        OXLPalHeader *header = (OXLPalHeader *)buf->base;
        uint32_t msgID = header->msgID;
        
        /* endian test follows in switch statement */
        /* *(int *)buf->base; */
        uint32_t msgLen;
        uint32_t msgRef;
        
        switch (msgID) {
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
                msgLen = header->msgLen;
                msgRef = header->msgRef;
                OXLPalClientLogon(client, msgLen, msgRef);
                break;
            case PAL_RX_BIG_ENDIAN_SERVER: /* MSG_TIYID */
                OXLLog("Server is big endian.");
                client->serverIsBigEndianFlag = 1;
                msgLen = header->msgLen; /* ntohl(header->msgLen); */
                msgRef = header->msgRef; /* ntohl(header->msgRef); */
                OXLPalClientLogon(client, msgLen, msgRef);
                break;
            default:
                OXLLog("Unexpected error while logging in.");
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
    OXLLog("HandleServerVersion");
    
    client->serverVersion = msgRef;
    OXLLog("Palace server ver %u", client->serverVersion);
    
    /* emit palace server version received event signal */
}

static void OXLPalClientHandleConnErr(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef)
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
    client->event.PalEventConnectFail(client);
}

void OXLPalClientHandleAltLogon(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef, const uv_buf_t *buf)
{
    OXLPalLogonCmd *altLogonCmd = (OXLPalLogonCmd *)buf->base;
    if (altLogonCmd->puidCounter != client->puidCounter ||
        altLogonCmd->puidCRC != client->puidCRC)
    {
        OXLLog("PUID changed by server.");
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

void OXLPalClientHandleXTalk(OXLPalClient *client, uint32_t msgLen, uint32_t msgRef)
{
    
    /* client->event.PalEventXTalk(client, msg); */
}

void OXLPalClientFinishDisconnect(uv_handle_t *handle)
{
    OXLPalClient *client = handle->data;
    client->state = STATE_DISCONNECTED;
}

void OXLPalClientDisconnect(OXLPalClient *client)
{
    OXLLog("Disconnecting from Palace %s", client->servername);
    /* OXLPalClient *client = handle->data; */
    /* oxl_client_t *client = (oxl_client_t *)handle->data; */
    client->conn->data = client;
    uv_close((uv_handle_t *)client->conn, OXLPalClientFinishDisconnect);
}

/* uv_read_cb */
void OXLPalClientFinishReadData(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    uint32_t msgID = 0;
    uint32_t msgLen;
    uint32_t msgRef;
    OXLPalClient *client = stream->data;
    
    OXLLog("FinishReadData");
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
        OXLBufDumpWithSize(*buf, nread);
        
        if (client->state == STATE_HANDSHAKING) {
            OXLLog("STATE_HANDSHAKING is true. Performing handshake.");
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
                    OXLLog("Read message ALTERNATE_LOGON_REPLY.");
                    OXLPalClientHandleAltLogon(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_CONNECTION_ERROR:
                    OXLLog("Read message CONNECTION_ERROR.");
                    OXLPalClientHandleConnErr(client, msgLen, msgRef);
                    break;
                case PAL_RX_SERVER_VERSION:
                    OXLLog("Read message SERVER_VERSION.");
                    OXLPalClientHandleServerVersion(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_SERVER_INFO:
                    OXLLog("DEBUG: Read message SERVER_INFO.");
                    OXLPalClientHandleServerInfo(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_USER_STATUS:
                    OXLLog("Read message USER_STATUS.");
                    OXLPalClientHandleUserStatus(client, msgLen, msgRef, buf);
                    break;
                case PAL_RX_USER_LOGGED_ON_AND_MAX:
                    OXLLog("Read message USER_LOGGED_ON_AND_MAX.");
                    OXLPalClientHandleMaxUsersLoggedOn(client, msgLen, msgRef, buf);
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
                    OXLLog("Read message PING.");
                    break;
                case PAL_RX_XTALK:
                    OXLLog("Read message XTALK.");
                    OXLPalClientHandleXTalk(client, msgLen, msgRef, buf);
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
                case PAL_RX_DRAW_CMD:
                    OXLLog("Read message DRAW_CMD.");
                    break;
                case PAL_RX_INCOMING_FILE:
                    OXLLog("Read message INCOMING_FILE.");
                    break;
                case PAL_RX_NAV_ERROR:
                    OXLLog("Read message NAV_ERROR.");
                    break;
                case PAL_RX_AUTHENTICATE:
                    OXLLog("Read message AUTHENTICATE.");
                    break;
                case PAL_RX_BLOWTHRU:
                    OXLLog("Read message BLOWTHRU.");
                    break;
                default:
                    OXLLog("Received unmatched message: "
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
            OXLLog("STATE_DISCONNECTED. Ignoring data.");
        }
    }
    OXLLog("FinishReadData: Freeing buf->base");
    if (buf->base) {
        free(buf->base);
    }
}

/* uv_connect_cb */
void OXLPalClientFinishConnect(uv_connect_t *conn, int32_t status)
{
    OXLLog("FinishConnect");
    OXLPalClient *client = conn->data;
    
    if (status) { /* error condition */
        OXLLog("Error: %d. %s\n", status, uv_strerror(status));
        OXLPalClientDisconnect(client);
        client->event.PalEventConnectFail(client);
    }
    else {
        OXLLog("Connected successfully.");
        client->state = STATE_HANDSHAKING;
        conn->handle->data = client;
        uv_read_start(conn->handle, OXLBufAlloc, OXLPalClientFinishReadData);
    }
    
    client->event.PalEventConnectSuccess(client);
}

/* getaddrinfo_cb */
void OXLPalClientFinishResolve(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    OXLLog("ResolveCallback");
    OXLPalClient *client = (OXLPalClient *)req->data;
    
    if (-1 == status) {
        OXLLog("getaddrinfo callback error %s", uv_strerror(status));
    }
    else {
        OXLLog("Resolve status OK.");
        
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
        uv_tcp_connect(conn_req, conn, (struct sockaddr *)res->ai_addr, OXLPalClientFinishConnect);
    }
    uv_freeaddrinfo(res);
}

/* uv_write_cb */
void OXLPalClientFinishWrite(uv_write_t *req, int status)
{
    OXLWriteReq *wr = (OXLWriteReq *)req;
    
    if (0 <= status) {
        OXLLog("Wrote %s", req->data);
    }
    
    free(wr->buf.base);
    free(wr);
}

OXLPalClient *OXLPalClientCreate()
{
    OXLPalClient *client = malloc(sizeof(*client));
    
    /* client->crypto = *OXLPalCryptoCreate(); */
    client->loop = uv_default_loop();
    client->puidChangedFlag = 0;
    client->puidCounter = 0xf5dc385e;
    client->puidCRC = 0xc144c580;
    client->regCounter = 0xcf07309c;
    client->regCRC = 0x5905f923;
    
    client->event.PalEventAuthRequestSuccess = NULL;
    client->event.PalEventConnectFail = NULL;
    client->event.PalEventConnectSuccess = NULL;
    client->event.PalEventDisconnectSuccess = NULL;
    client->event.PalEventGotoURLSuccess = NULL;
    client->event.PalEventLogonFail = NULL;
    client->event.PalEventLogonSuccess = NULL;
    client->event.PalEventRoomChangeSuccess = NULL;
    client->event.PalEventSecurityFail = NULL;
    client->event.PalEventServerInfo = NULL;
    
    return client;
}

void OXLPalClientDestroy(OXLPalClient *client)
{
    /* OXLPalCryptoDestroy(&client->crypto); */
    OXLPalClientDisconnect(client);
    uv_loop_close(client->loop);
    free(client->loop);
    free(client);
}

void OXLPalClientConnect(OXLPalClient *client,
                         const char *username,
                         const char *wizpass,
                         const char *host,
                         const uint16_t port,
                         const int32_t initialRoomID)
{
    size_t usernameLen = strlen(username) + 1;
    size_t wizpassLen = strlen(wizpass) + 1;
    
    /* client->username = calloc(z, PAL_MAX_USERNAME_LEN); */
    /* sizeof *client->username); */
    memset(&client->username, '\0', PAL_USERNAME_SZ_CAP);
    strlcpy(client->username, username, usernameLen);
    memset(&client->wizpass, '\0', PAL_WIZ_PASS_SZ_CAP);
    strlcpy(client->wizpass, wizpass, wizpassLen);
    client->roomID = initialRoomID;
    
    char portString[IP_PORT_SZ_CAP];
    OXLInt2Str(port, portString, IP_PORT_SZ_CAP);
    if (port == 9998) {
        OXLLog("Connecting to palace://%s as %s", host, username);
    }
    else {
        OXLLog("Connecting to %s:%n as %s", host, portString, username);
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
                               OXLPalClientFinishResolve,
                               host,
                               portString,
                               hints);
    
    OXLLog("After uv_getaddrinfo");
    if (r) {
        OXLLog("ERROR getaddrinfo call returned: %s", uv_strerror(r));
        return;
    }
}

void OXLPalClientFinishJoinRoom()
{
    OXLLog("FinishJoinRoom");
    /* TODO emit room joined event signal */
}

void OXLPalClientJoinRoom(OXLPalClient *client, int32_t gotoRoomID)
{
    if (client->state != STATE_CONNECTED) {
        return;
    }
    
    OXLWriteReq *logonCmd = malloc(sizeof *logonCmd);
    OXLLog("Allocated write_req");
    
    logonCmd->req.data = client;
    
    int32_t z = sizeof(OXLPalGotoRoomCmd);
    OXLLog("sizeof(OXL_GotoPalRoomCmd) is %d", z);
    logonCmd->buf = uv_buf_init(malloc(z), z);
    OXLLog("uv_buf_init(malloc(%d), sizeof(%d));", z, z);
    
    OXLPalGotoRoomCmdInit((OXLPalGotoRoomCmd *)logonCmd->buf.base, client->user.id, (short)gotoRoomID);
    
    client->currentRoom.selectedUser = NULL;
    
    OXLLog("");
    OXLLog("Will write the following:");
    /* OXLBufDump(logonCmd->buf, z);*/
    OXLBufDump(logonCmd->buf);
    
    OXLLog("Calling uv_write");
    uv_write((uv_write_t *)logonCmd, (uv_stream_t *)client->conn, &logonCmd->buf, 1, OXLPalClientFinishJoinRoom);
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
    OXLLog("OXLPalSay");
    OXLLog("Received buf from app:");
    /* OXLBufDump(buf, size); */
    OXLBufDump(buf);
    
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
    
    OXLLog("Preparing to send the following ciphertext:");
    /* OXLBufDump(wr->buf, size); */
    OXLBufDump(wr->buf);
    //uv_write((uv_stream_t *)client->conn, size, wr->buf, finishWrite);
}

void OXLSetPalUsername(OXLPalClient *client, char *username)
{
    strlcpy(client->username, username, PAL_USERNAME_SZ_CAP);
}

