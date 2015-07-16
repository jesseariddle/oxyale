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

/* uv_alloc_cb */
void OXLAllocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

void OXLClientFinishPalaceLogon(uv_write_t *req, int status)
{
    fprintf(stderr, "--- DEBUG: oxl_client_do_logon_cb\n");
    write_req_t *wr = (write_req_t *)req;
    oxl_client_t *client = wr->req.data;

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
}

void OXLPalaceLogon(OXLPalaceClient *client, uint size, uint refID)
{
    fprintf(stderr, "--- DEBUG: OXLPalaceLogon(0x%x, 0x%x, 0x%x)\n", (uint)client, size, refID);

    client->room.userID = refID;
    fprintf(stderr, "--- DEBUG: client->currentRoom.userID = 0x%x;\n", refID);

    write_req_t *logonMsg = malloc(sizeof *logonMsg);
    fprintf(stderr, "--- DEBUG: allocated write_req for logonMsg\n");

    logonMsg->req.data = client;

    int z = sizeof(OXLPalaceLogonMsg);
    fprintf(stderr, "--- DEBUG: sizeof(OXLPalaceLogonMsg) is %d\n", z);
    logonMsg->buf = uv_buf_init(malloc(z), z);
    fprintf(stderr, "--- DEBUG: uv_buf_init(malloc(%d), sizeof(%d));\n", z, z);

    int i = 0;
    fprintf(stderr, "\n--- DEBUG: logon_wr->buf.base[%d]: %08x\n\n", i, logonMsg->buf.base[i]);

    InitLogon((OXLPalaceLogonCommand *)logonMsg->buf.base,
              client->username,
              client->wizpass,
              client->roomID,
              client->regCRC,
              client->regCounter,
              client->PUIDCRC,
              client->PUIDCounter);

    fprintf(stderr, "--- DEBUG: before DumpPalLogonMsg\n");
    OXLDumpPalaceLogonMsg((OXLPalaceLogonCommand *)logonMsg->buf.base);
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
            (uint)&write_req,
            (uint)client->conn,
            (uint)&logonMsg,
            (uint)&finishLogon);
    */
    fprintf(stderr, "--- DEBUG: we are about to write the following:\n");
    DumpPalBuf(logonMsg->buf, z);

    /* DumpPalBufPtr((uv_buf_t *)logonMsg->buf, z); */

    fprintf(stderr, "--- DEBUG: calling uv_write\n");
    uv_write((uv_write_t *)logonMsg, (uv_stream_t *)client->conn, &logonMsg->buf, 1, finishLogon);
    /*
    write_data((uv_stream_t *)client->conn,
               z,
               logonMsg,
               finishLogon);
    */
}

void OXLPalaceHandshake(OXLPalaceClient *client, ssize_t nread, const uv_buf_t *buf)
{
    fprintf(stderr, "--- DEBUG: handshake\n");
    if (0 < nread) {
        fprintf(stderr, "--- DEBUG: %ld bytes read\n", nread);
        PalHeader *header = (PalHeader *)buf->base;
        uint msgID = header->msgID;

        /* endian test follows in switch statement */
        /* *(int *)buf->base; */
        uint msgLen;
        uint msgRef;

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
            OXLPalaceLogon(client, msgLen, msgRef);
            break;
        case PAL_RX_BIG_ENDIAN_SERVER: /* MSG_TIYID */
            fprintf(stderr, "--- DEBUG: Server is big endian.\n");
            client->serverIsBigEndianFlag = 1;
            msgLen = header->msgLen; /* ntohl(header->msg_len); */
            msgRef = header->msgRef; /* ntohl(header->msg_ref); */
            logon(client, msgLen, msgRef);
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

static void OXL_finishJoinPalRoom()
{

    /* TODO emit room joined event signal */
}

void OXL_JoinPalRoom(PalClient *client, uint gotoRoomID)
{
    if (client->state != STATE_CONNECTED) return;

    write_req_t *logonMsg = malloc(sizeof *logonMsg);
    fprintf(stderr, "--- DEBUG: allocated write_req\n");

    logonMsg->req.data = client;

    int z = sizeof(OXL_GotoPalRoomCmd);
    fprintf(stderr, "--- DEBUG: sizeof(OXL_GotoPalRoomCmd) is %d\n", z);
    logonCmd->buf = uv_buf_init(malloc(z), z);
    fprintf(stderr, "--- DEBUG: uv_buf_init(malloc(%d), sizeof(%d));\n", z, z);

    InitGotoRoomMsg((OXL_GotoPalRoomCmd *)logonCmd->buf.base, client->user.id, (short)gotoPalRoomID);

    client->room.selectedUser = NULL;

    fprintf(stderr, "--- DEBUG: we are about to write the following:\n");
    OXL_DumpPalBuf(logonCmd->buf, z);

    /* oxl_dump_buf_p((uv_buf_t *)logon_wr->buf, z); */
    fprintf(stderr, "--- DEBUG: calling uv_write\n");
    uv_write((uv_write_t *)logonCmd,
             (uv_stream_t *)client->conn,
             &logonCmd->buf,
             1,
             OXL_finishJoinPalRoom);
}

void OXL_leavePalRoom(OXL_PalClient *client)
{
    /* Let all iptscrae ON LEAVE event handlers execute */
    /* TODO emit leave room event signal */
}

void OXL_LeavePalRoom(OXL_PalClient *client)
{
    leaveRoom(client);
}

void OXL_GotoPalRoom(OXL_PalClient *client, uint gotoRoomID)
{
    if (client->state != STATE_CONNECTED || client->roomID == gotoRoomID) {
        return;
    }

    OXL_leavePalRoom(client);
    OXL_joinPalRoom(client, gotoRoomID);
}

static void OXL_handlePalServerVersion(OXL_PalClient *client,
                                       uint msgLen,
                                       uint msgRef,
                                       const uv_buf_t *buf)
{
    fprintf(stderr, "--- DEBUG: handleServerVersion\n");

    client->serverVersion = msgRef;
    fprintf(stderr, "--- DEBUG: palace server version %u\n", client->serverVersion);

    /* emit palace server version received event signal */
}

static void OXL_handleConnErr(PalClient *client, uint msgLen, uint msgRef)
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
                client->PUIDChangedFlag, msgLen, msgRef);
        break;
    }

    if (!client->PUIDChangedFlag) {
        fprintf(stderr, "--- DEBUG: puid unchanged. connection dropped.\n");
    }

    /* TODO emit connection error received event signal */
}

void OXL_handleAltLogon(OXL_PalClient *client, uint msgLen, uint msgRef, const uv_buf_t *buf)
{
    OXL_PalLogonCmd *altLogonCmd = (OXL_PalLogonCmd *)buf->base;
    if (altLogonCmd->PUIDCounter != client->PUIDCounter || altLogonCmd->PUIDCRC != client->PUIDCRC)
    {
        fprintf(stderr, "PUID changed by server.\n");
        client->PUIDCRC = altLogonCmd->PUIDCRC;
        client->PUIDCounter = altLogonCmd->PUIDCounter;
        client->PUIDChangedFlag = 1;
    }

    /* TODO emit alt_logon received event signal */
}

void OXL_handleServerVersion(OXL_PalClient *client, uint msgLen, uint msgRef)
{
    client->serverVersion = msgRef;
    
    /* TODO emit server version received event signal */
}

void OXL_handlePalServerInfoMsg(OXL_PalClient *client, uint msgLen, uint msgRef, const uv_buf_t *buf)
{
    PalServerInfoMsg *server_info = (OXL_PalServerInfoMsg *)buf;
    client->permissions = serverInfo->permissions;
    strlcpy(client->serverName, serverInfo->serverName, PAL_SERVER_NAME_CAP_SZ);

    /* TODO emit handle server info received event signal  */
}

void OXL_handlePalUserStatusMsg(OXL_PalClient *client, uint msgLen, uint msgRef, const uv_buf_t *buf)
{
    PalUser *user = (PalUser *)buf->base;
    if (-1 < client->user.id) {
        client->user.flags = user->flags;
    }
    /* TODO emit handle user status received event signal  */
}

void OXL_handlePalMaxUsersLoggedOnMsg(OXL_PalClient *client, uint msgLen, uint msgRef, const uv_buf_t *buf)
{
    /* TODO emit user logged on and max received event signal */
}

void OXL_finishClosePalConnection(uv_handle_t *handle)
{
    fprintf(stderr, "--- DEBUG: oxl_client_do_conn_close_cb()\n");
    oxl_client_t *client = handle->data;
    /* oxl_client_t *client = (oxl_client_t *)handle->data; */
    /* client->state = STATE_DISCONNECTED; */
    /* close socket? */
    /* uv_close(handle->data) */
    free(client);
}

/* uv_read_cb */
void OXL_finishReadPalData(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    uint msgID = 0;
    uint msgLen;
    uint msgRef;

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
        fprintf(stderr,
                "--- DEBUG: successfully read %ld bytes of data\n",
                nread);

        fprintf(stderr, "--- DEBUG: read the following:\n");
        fprintf(stderr, "--- DEBUG: ");
        OXL_DumpPalBuf(*buf, nread);

        OXL_PalClient *client = stream->data;
        if (client->state == STATE_HANDSHAKING) {
            fprintf(stderr,
                    "--- DEBUG: STATE_HANDSHAKING is true. "
                    "Performing handshake.\n");
            OXL_palHandshake((oxl_client_t *)stream->data, nread, buf);
        }
        else if (client->state == STATE_CONNECTED) {
            /* fprintf(stderr,
               "--- DEBUG: reading data with STATE_CONNECTED true.\n"); */

            if (client->msg_id == 0) {
                /* fprintf(stderr, "--- DEBUG: client->msg_id == 0\n"); */
                if (PAL_HEADER_LEN <= nread) {
                    oxl_header_t *header = (oxl_header_t *)buf->base;
                    /* *((int *)buf->base)); */
                    client->msg_id = header->msg_id;
                    /* *((int *)(buf->base + sizeof client->msg_id))); */
                    client->msg_len = header->msg_len;
                    /* *((int *)(buf->base + */
                    /* sizeof client->msg_id + sizeof client->msg_len))); */
                    client->msg_ref = header->msg_ref;
                    /* oxl_header_t *header = malloc(nread);
                    memcpy(header, buf->base, nread);
                    free(header); */ /* do something with it instead */
                }
                else {
                    return; /* TODO ? */
                }
            }

            msg_id = client->msg_id;
            msg_len = client->msg_len;
            msg_ref = client->msg_ref;

            if (nread < msg_len) {
                return; /* TODO ? */
            }

            switch (msg_id) {
            case PAL_RX_ALTERNATE_LOGON_REPLY:
                fprintf(stderr, "--- DEBUG: Read message ALTERNATE_LOGON_REPLY.\n");
                handleAltLogon(client, msg_len, msg_ref, buf);
                break;
            case PAL_RX_CONNECTION_ERROR:
                fprintf(stderr, "--- DEBUG: Read message CONNECTION_ERROR.\n");
                handleConnectionError(client, msg_len, msg_ref);
                break;
            case PAL_RX_SERVER_VERSION:
                fprintf(stderr, "--- DEBUG: Read message SERVER_VERSION.\n");
                handleServerVersion(client, msg_len, msg_ref);
                break;
            case PAL_RX_SERVER_INFO:
                fprintf(stderr, "--- DEBUG: Read message SERVER_INFO.\n");
                handleServerInfo(client, msg_len, msg_ref, buf);
                break;
            case PAL_RX_USER_STATUS:
                fprintf(stderr, "--- DEBUG: Read message USER_STATUS.\n");
                handleUserStatus(client, msg_len, msg_ref, buf);
                break;
            case PAL_RX_USER_LOGGED_ON_AND_MAX:
                fprintf(stderr, "--- DEBUG: Read message USER_LOGGED_ON_AND_MAX.\n");
                handleMaxUsersLoggedOn(client, msg_len, msg_ref, buf);
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
                            "[msg_id = 0x%x, "
                            "msg_len = 0x%x, "
                            "msg_ref = 0x%x]\n",
                            msg_id,
                            msg_len,
                            msg_ref);
                break;
            }
            client->msg_id = 0;
        }
        else if (client->state == STATE_DISCONNECTED)
        {
            /* disconnected, yeah? */
            fprintf(stderr,
                    "--- DEBUG: STATE_DISCONNECTED: freeing stream->data\n");
            free(stream->data);
            fprintf(stderr,
                    "--- DEBUG: STATE_DISCONNECTED: closing connection\n");
            uv_close((uv_handle_t *)stream, closePalConnectionCallback);
        }
    }

    fprintf(stderr, "--- DEBUG: oxl_client_read_cb free(buf->base)\n");
    if (buf->base) {
        free(buf->base);
    }
}

/* uv_connect_cb */
void openPalConnectionCallback(uv_connect_t *conn, int status)
{
    fprintf(stderr, "--- DEBUG: oxl_client_conn_open_cb\n");
    oxl_client_t *client = conn->data;

    if (status) { /* error condition */
        fprintf(stderr,
                "--- DEBUG: error: %d. %s\n",
                status,
                uv_strerror(status));

        uv_close((uv_handle_t *)conn->handle, closeConnectionCallback);
    }
    else {
        fprintf(stderr, "--- DEBUG: connect successful.\n");
        client->state = STATE_HANDSHAKING;
        conn->handle->data = client;
        uv_read_start(conn->handle, allocBuf, readCallback);
    }
}

/* getaddrinfo_cb */
static void resolvePalCallback(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    fprintf(stderr, "--- DEBUG: resolvePalCallback\n");
    oxl_client_t *client = (oxl_client_t *)req->data;

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
        uv_tcp_connect(conn_req,
                       conn,
                       (struct sockaddr *)res->ai_addr,
                       openPalConnectionCallback);
    }
    uv_freeaddrinfo(res);
}

/* uv_write_cb */
void writeCallback(uv_write_t *req, int status)
{
    write_req_t *wr = (write_req_t *)req;

    if (0 <= status) {
        fprintf(stderr, "--- DEBUG: wrote: %s\n", req->data);
    }

    free(wr->buf.base);
    free(wr);
}

void ClosePalConnection(oxl_client_t *client) {
    fprintf(stderr, "--- DEBUG: CloseConnection\n");
    /* free memory here */
    /* close connection? */
    /* free(client->host); */
}

void OpenPalConnection(oxl_client_t *client,
                    uv_loop_t *loop,
                    char *username,
                    char *wizpass,
                    char *host,
                    uint port,
                    uint initial_room_id)
{
    fprintf(stderr, "--- DEBUG: OpenConnection\n");

    /* init */
    InitPalCrypt(&client->crypt);
    client->loop = loop;
    client->is_puid_changed = 0;
    client->puid_counter = 0xf5dc385e;
    client->puid_crc = 0xc144c580;
    client->reg_counter = 0xcf07309c;
    client->reg_crc = 0x5905f923;

    /* start */
    size_t z_username = strlen(username) + 1;
    size_t z_wizpass = strlen(wizpass) + 1;

    /* client->username = calloc(z, PAL_MAX_USERNAME_LEN); */
        /* sizeof *client->username); */
    memset(&client->username, '\0', PAL_USERNAME_SZ_CAP);
    strlcpy(client->username, username, z_username);
    memset(&client->wizpass, '\0', PAL_WIZ_PASS_SZ_CAP);
    strlcpy(client->wizpass, wizpass, z_wizpass);
    client->room_id = initial_room_id;

    char port_str[PAL_INET_PORT_SZ_CAP];
    snprintf(port_str, PAL_INET_PORT_SZ_CAP - 1, "%d", port);
    fprintf(stderr, "--- DEBUG: host: %s\n", host);
    fprintf(stderr, "--- DEBUG: port: %s\n", port_str);

    struct addrinfo *hints = malloc(sizeof *hints);
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;

    /* fprintf(stderr, "--- DEBUG: before uv_getaddrinfo\n"); */
    uv_getaddrinfo_t *resolver = malloc(sizeof *resolver);
    resolver->data = client;

    int r = uv_getaddrinfo(loop,
                           resolver,
                           oxl_client_do_resolve_cb,
                           host,
                           port_str,
                           hints);

    /* fprintf(stderr, "--- DEBUG: after uv_getaddrinfo\n"); */
    if (r) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_strerror(r));
        return;
    }
}

void PalSay(oxl_client_t *client,
            size_t size,
            uv_buf_t buf,
            uv_write_cb write_cb)
{
    fprintf(stderr, "--- DEBUG: oxl_client_say\n");
    fprintf(stderr, "--- DEBUG: received buf from app:\n");
    oxl_dump_buf(buf, size);

    write_req_t *wr = malloc(sizeof *wr);
    wr->req.data = client;

    oxl_say_bubble_t *bubble = malloc(sizeof *bubble);

    int z = sizeof(oxl_say_t);
    wr->buf = uv_buf_init(malloc(z), z);
    oxl_say_init((oxl_say_t *)wr->buf.base, client, buf.base);

    oxl_say_dump((oxl_say_t *)wr->buf.base);

    fprintf(stderr,
            "--- DEBUG: preparing to send the following ciphertext:\n");
    oxl_dump_buf(wr->buf, size);
    write_data((uv_stream_t *)client->conn, size, wr->buf, write_cb);
}

void SetUsername(oxl_client_t *client, char *username) {
    strlcpy(client->username, username, PAL_USERNAME_SZ_CAP);
}
