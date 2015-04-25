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
void oxl_client_do_alloc_buffer(uv_handle_t *handle,
                                size_t suggested_size,
                                uv_buf_t *buf)
{
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

void oxl_client_do_logon_cb(uv_write_t *req, int status) {
    fprintf(stderr, "--- DEBUG: oxl_client_do_logon_cb\n");
    write_req_t *wr = (write_req_t *)req;
    oxl_client_t *self = wr->req.data;

    if (status < 0) {
        fprintf(stderr,
                "--- DEBUG: error condition. "
                "STATE_DISCONNECTED is now true.\n");
        
        self->state = STATE_DISCONNECTED;
        uv_close((uv_handle_t *)self->conn, NULL);
    }
    else if (0 <= status) {
        fprintf(stderr,
                "--- DEBUG: write successful. STATE_CONNECTED is now true.\n");
        
        self->state = STATE_CONNECTED;
    }

    if (wr) {
        if (wr->buf.base) {
            free(wr->buf.base);
        }
        free(wr);
    }
}

void oxl_client_do_logon(oxl_client_t *self,
                         unsigned int size,
                         unsigned int reference_id) {
    fprintf(stderr,
            "--- DEBUG: oxl_client_do_logon(0x%x, 0x%x, 0x%x)\n",
            (unsigned int)self,
            size,
            reference_id);

    self->room.user_id = reference_id;
    fprintf(stderr,
            "--- DEBUG: self->current_room.user_id = 0x%x;\n",
            reference_id);

    write_req_t *logon_wr = malloc(sizeof *logon_wr);
    fprintf(stderr, "--- DEBUG: allocated write_req\n");

    logon_wr->req.data = self;

    int z = sizeof(oxl_net_logon_t);
    fprintf(stderr, "--- DEBUG: sizeof(oxl_net_logon_t) is %d\n", z);
    logon_wr->buf = uv_buf_init(malloc(z), z);
    fprintf(stderr, "--- DEBUG: uv_buf_init(malloc(%d), sizeof(%d));\n", z, z);

    int i = 0;
    fprintf(stderr,
            "\n--- DEBUG: logon_wr->buf.base[%d]: %08x\n\n",
            i,
            logon_wr->buf.base[i]);
    
    oxl_net_logon_init((oxl_net_logon_t *)logon_wr->buf.base,
                   self->username,
                   self->wizpass,
                   self->room_id,
                   self->reg_crc,
                   self->reg_counter,
                   self->puid_crc,
                   self->puid_counter);

    fprintf(stderr, "--- DEBUG: before oxl_net_logon_dump\n");
    oxl_net_logon_dump((oxl_net_logon_t *)logon_wr->buf.base);
    fprintf(stderr, "--- DEBUG: after oxl_net_logon_dump\n");
/*
    fprintf(stderr,
            "--- DEBUG: oxl_net_logon("
            "%s, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, &logon_msg);\n",
            self->username,
            self->room_id,
            self->reg_crc,
            self->reg_counter,
            self->puid_crc,
            self->puid_counter);
*/    
    /* endianness? */
    /*
    fprintf(stderr,
            "--- DEBUG: uv_write(0x%x (write_req), 0x%x (self->conn), "
            "0x%x (&logon_msg), 1, 0x%x);\n",
            (unsigned int)&write_req,
            (unsigned int)self->conn,
            (unsigned int)&logon_msg,
            (unsigned int)&oxl_client_do_logon_cb); 
    */
    fprintf(stderr, "--- DEBUG: we are about to write the following:\n");
    oxl_dump_buf(logon_wr->buf, z);
    
    /* oxl_dump_buf_p((uv_buf_t *)logon_wr->buf, z); */

    fprintf(stderr, "--- DEBUG: calling uv_write\n");
    uv_write((uv_write_t *)logon_wr,
             (uv_stream_t *)self->conn,
             &logon_wr->buf, 1,
             oxl_client_do_logon_cb);
    /*
    write_data((uv_stream_t *)self->conn,
               z,
               logon_msg,
               oxl_client_do_logon_cb);
    */
}

void oxl_client_do_handshake(oxl_client_t *self,
                             ssize_t nread,
                             const uv_buf_t *buf)
{
    fprintf(stderr, "--- DEBUG: oxl_client_do_handshake\n");
    if (0 < nread) {
        fprintf(stderr, "--- DEBUG: %ld bytes read\n", nread);
        oxl_net_header_t *header = (oxl_net_header_t *)buf->base;
        int msg_id = header->msg_id;
        
        /* endian test follows in switch statement */
        /* *(int *)buf->base; */ 
        int msg_len;
        int msg_ref;

        switch (msg_id) {
        case OXL_NET_RX_UNKNOWN_SERVER:
            fprintf(stderr, "--- DEBUG: MSG_TROPSER. execution stops here.\n");
            break;
        case OXL_NET_RX_LITTLE_ENDIAN_SERVER: /* MSG_DIYIT */
            fprintf(stderr, "--- DEBUG: Server is little endian.\n");
            /* TODO will a server EVER send little endian data? */
            /* Big endian is standard for network communication. */
            self->is_server_big_endian = 0;
            /* TODO need to accomodate for endianness? */
            /* i.e. little endian server, big endian client. */
            msg_len = header->msg_len;
            msg_ref = header->msg_ref;
            oxl_client_do_logon(self, msg_len, msg_ref);
            break;
        case OXL_NET_RX_BIG_ENDIAN_SERVER: /* MSG_TIYID */
            fprintf(stderr, "--- DEBUG: Server is big endian.\n");
            self->is_server_big_endian = 1;
            msg_len = header->msg_len; /* ntohl(header->msg_len); */
            msg_ref = header->msg_ref; /* ntohl(header->msg_ref); */
            oxl_client_do_logon(self, msg_len, msg_ref);
            break;
        default:
            fprintf(stderr, "--- DEBUG: unexpected error while logging in.\n");
            break;
        }
        /* msg_id = req->data */
    }
    else {
        /* TODO possible error? no bytes read. */
    }
}

void oxl_client_do_join_room_cb()
{
    
    /* TODO emit room joined event signal */
}

void oxl_client_do_join_room(oxl_client_t *self, unsigned int goto_room_id)
{
    if (self->state != STATE_CONNECTED) return;

    write_req_t *logon_wr = malloc(sizeof *logon_wr);
    fprintf(stderr, "--- DEBUG: allocated write_req\n");

    logon_wr->req.data = self;

    int z = sizeof(oxl_net_goto_room_t);
    fprintf(stderr, "--- DEBUG: sizeof(oxl_net_join_room_t) is %d\n", z);
    logon_wr->buf = uv_buf_init(malloc(z), z);
    fprintf(stderr, "--- DEBUG: uv_buf_init(malloc(%d), sizeof(%d));\n", z, z);

    oxl_net_goto_room_init((oxl_net_goto_room_t *)logon_wr->buf.base,
                           self->user.id,
                           (short)goto_room_id);
                           
    self->room.selected_user = NULL;

    fprintf(stderr, "--- DEBUG: we are about to write the following:\n");
    oxl_dump_buf(logon_wr->buf, z);
    
    /* oxl_dump_buf_p((uv_buf_t *)logon_wr->buf, z); */
    fprintf(stderr, "--- DEBUG: calling uv_write\n");
    uv_write((uv_write_t *)logon_wr,
             (uv_stream_t *)self->conn,
             &logon_wr->buf,
             1,
             oxl_client_do_join_room_cb);
}

void oxl_client_leave_room(oxl_client_t *self)
{
    /* Let all iptscrae ON LEAVE event handlers execute */
    /* TODO emit leave room event signal */
}

void oxl_client_goto_room(oxl_client_t *self, unsigned int goto_room_id)
{
    if (self->state != STATE_CONNECTED || self->room_id == goto_room_id) {
        return;
    }
    
    oxl_client_do_leave_room(self);
    oxl_client_do_join_room(self, goto_room_id);
}

void oxl_client_do_handle_palace_server_version(oxl_client_t *self,
                                                int msg_len,
                                                int msg_ref,
                                                const uv_buf_t *buf)
{
    fprintf(stderr,
            "--- DEBUG: oxl_client_do_handle_palace_server_version\n");
    
    self->server_version = msg_ref;
    fprintf(stderr,
            "--- DEBUG: palace server version %u\n",
            self->server_version);

    /* emit palace server version received event signal */
}

void oxl_client_do_handle_conn_err(oxl_client_t *self,
                                   int msg_len,
                                   int msg_ref)
{
    fprintf(stderr, "--- DEBUG: oxl_client_do_handle_conn_err\n");
    switch (msg_ref) {
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
                "[is_puid_changed: 0x%x, msg_len: 0x%x, msg_ref: 0x%x]\n",
                self->is_puid_changed, msg_len, msg_ref);
        break;
    }

    if (!self->is_puid_changed) {
        fprintf(stderr, "--- DEBUG: puid unchanged. connection dropped.\n");
    }

    /* TODO emit connection error received event signal */
}

void oxl_client_do_handle_alt_logon(oxl_client_t *self,
                                    unsigned int msg_len,
                                    unsigned int msg_ref,
                                    const uv_buf_t *buf)
{
    oxl_net_logon_t *altlogon = (oxl_net_logon_t *)buf->base;
    if (altlogon->puid_counter != self->puid_counter ||
        altlogon->puid_crc != self->puid_crc)
    {
        fprintf(stderr, "PUID changed by server.\n");
        self->puid_crc = altlogon->puid_crc;
        self->puid_counter = altlogon->puid_counter;
        self->is_puid_changed = 1;
    }

    /* TODO emit alt_logon received event signal */
}

void oxl_client_do_handle_server_version(oxl_client_t *self,
                                         unsigned int msg_len,
                                         unsigned int msg_ref)
{
    self->server_version = msg_ref;
    
    /* TODO emit server version received event signal */
}

void oxl_client_do_handle_server_info(oxl_client_t *self,
                                      unsigned int msg_len,
                                      unsigned int msg_ref,
                                      const uv_buf_t *buf)
{
    oxl_server_info_t *server_info = (oxl_server_info_t *)buf;
    self->permissions = server_info->permissions;
    strlcpy(self->server_name,
            server_info->server_name,
            OXL_CLIENT_SERVER_NAME_SZ_CAP);

    /* TODO emit handle server info received event signal  */
}

void oxl_client_do_handle_user_status(oxl_client_t *self,
                                      unsigned int msg_len,
                                      unsigned int msg_ref,
                                      const uv_buf_t *buf)
{
    oxl_user_t *user = (oxl_user_t *)buf->base;
    if (-1 < self->user.id) {
        self->user.flags = user->flags;
    }
    /* TODO emit handle user status received event signal  */
}

void oxl_client_do_handle_user_logged_on_and_max(oxl_client_t *self,
                                                 unsigned int msg_len,
                                                 unsigned int msg_ref,
                                                 const uv_buf_t *buf)
{
    /* TODO emit user logged on and max received event signal */
}

void oxl_client_do_conn_close_cb(uv_handle_t *handle)
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
void oxl_client_do_read_cb(uv_stream_t *stream,
                           ssize_t nread,
                           const uv_buf_t *buf)
{
    unsigned int msg_id = 0;
    unsigned int msg_len;
    unsigned int msg_ref;

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
        oxl_dump_buf(*buf, nread);

        oxl_client_t *client = stream->data;
        if (client->state == STATE_HANDSHAKING) {
            fprintf(stderr,
                    "--- DEBUG: STATE_HANDSHAKING is true. "
                    "Performing handshake.\n");
            oxl_client_do_handshake((oxl_client_t *)stream->data, nread, buf);
        }
        else if (client->state == STATE_CONNECTED) {
            /* fprintf(stderr,
               "--- DEBUG: reading data with STATE_CONNECTED true.\n"); */
            
            if (client->msg_id == 0) {
                /* fprintf(stderr, "--- DEBUG: client->msg_id == 0\n"); */
                if (OXL_NET_HEADER_LEN <= nread) {
                    oxl_net_header_t *header = (oxl_net_header_t *)buf->base;
                    /* *((int *)buf->base)); */
                    client->msg_id = header->msg_id;
                    /* *((int *)(buf->base + sizeof client->msg_id))); */
                    client->msg_len = header->msg_len;
                    /* *((int *)(buf->base + */
                    /* sizeof client->msg_id + sizeof client->msg_len))); */
                    client->msg_ref = header->msg_ref;
                    /* oxl_net_header_t *header = malloc(nread);
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
            case OXL_NET_RX_ALTERNATE_LOGON_REPLY:
                fprintf(stderr,
                        "--- DEBUG: Read message ALTERNATE_LOGON_REPLY.\n");
                
                oxl_client_do_handle_alt_logon(client, msg_len, msg_ref, buf);
                break;
            case OXL_NET_RX_CONNECTION_ERROR:
                fprintf(stderr, "--- DEBUG: Read message CONNECTION_ERROR.\n");
                oxl_client_do_handle_conn_err(client, msg_len, msg_ref);
                break;
            case OXL_NET_RX_SERVER_VERSION:
                fprintf(stderr, "--- DEBUG: Read message SERVER_VERSION.\n");
                oxl_client_do_handle_server_version(client, msg_len, msg_ref);
                break;
            case OXL_NET_RX_SERVER_INFO:
                fprintf(stderr, "--- DEBUG: Read message SERVER_INFO.\n");
                oxl_client_do_handle_server_info(client,
                                                 msg_len,
                                                 msg_ref,
                                                 buf);
                break;
            case OXL_NET_RX_USER_STATUS:
                fprintf(stderr, "--- DEBUG: Read message USER_STATUS.\n");
                oxl_client_do_handle_user_status(client,
                                                 msg_len,
                                                 msg_ref,
                                                 buf);
                break;
            case OXL_NET_RX_USER_LOGGED_ON_AND_MAX:
                fprintf(stderr,
                        "--- DEBUG: Read message USER_LOGGED_ON_AND_MAX.\n");
                oxl_client_do_handle_user_logged_on_and_max(client,
                                                            msg_len,
                                                            msg_ref,
                                                            buf);
                break;
            case OXL_NET_RX_GOT_HTTP_SERVER_LOCATION:
                fprintf(stderr,
                        "--- DEBUG: Read message GOT_HTTP_SERVER_LOCATION.\n");
                break;
            case OXL_NET_RX_GOT_ROOM_DESCRIPTION:
                fprintf(stderr,
                        "--- DEBUG: Read message GOT_ROOM_DESCRIPTION.\n");
                break;
            case OXL_NET_RX_GOT_ROOM_DESCRIPTION_ALT:
                fprintf(stderr,
                        "--- DEBUG: Read message GOT_ROOM_DESCRIPTION_ALT.\n");
                break;
            case OXL_NET_RX_GOT_USER_LIST:
                fprintf(stderr,
                        "--- DEBUG: Read message GOT_USER_LIST.\n");
                break;
            case OXL_NET_RX_GOT_REPLY_OF_ALL_USERS:
                fprintf(stderr,
                        "--- DEBUG: Read message GOT_REPLY_OF_ALL_USERS.\n");
                break;
            case OXL_NET_RX_GOT_ROOM_LIST:
                fprintf(stderr, "--- DEBUG: Read message GOT_ROOM_LIST.\n");
                break;
            case OXL_NET_RX_ROOM_DESCEND:
                fprintf(stderr, "--- DEBUG: Read message ROOM_DESCEND.\n");
                break;
            case OXL_NET_RX_USER_NEW:
                fprintf(stderr, "--- DEBUG: Read message USER_NEW.\n");
                break;
            case OXL_NET_RX_PING:
                fprintf(stderr, "--- DEBUG: Read message PING.\n");
                break;
            case OXL_NET_RX_XTALK:
                fprintf(stderr, "--- DEBUG: Read message XTALK.\n");
                break;
            case OXL_NET_RX_XWHISPER:
                fprintf(stderr, "--- DEBUG: Read message XWHISPER.\n");
                break;
            case OXL_NET_RX_TALK:
                fprintf(stderr, "--- DEBUG: Read message TALK.\n");
                break;
            case OXL_NET_RX_WHISPER:
                fprintf(stderr, "--- DEBUG: Read message WHISPER.\n");
                break;
            case OXL_NET_RX_ASSET_INCOMING:
                fprintf(stderr, "--- DEBUG: Read message ASSET_INCOMING.\n");
                break;
            case OXL_NET_RX_ASSET_QUERY:
                fprintf(stderr, "--- DEBUG: Read message ASSET_QUERY.\n");
                break;
            case OXL_NET_RX_MOVEMENT:
                fprintf(stderr, "--- DEBUG: Read message MOVEMENT.\n");
                break;
            case OXL_NET_RX_USER_COLOR:
                fprintf(stderr, "--- DEBUG: Read message USER_COLOR.\n");
                break;
            case OXL_NET_RX_USER_FACE:
                fprintf(stderr, "--- DEBUG: Read message USER_FACE.\n");
                break;
            case OXL_NET_RX_USER_PROP:
                fprintf(stderr, "--- DEBUG: Read message USER_PROP.\n");
                break;
            case OXL_NET_RX_USER_DESCRIPTION:
                fprintf(stderr, "--- DEBUG: Read message USER_DESCRIPTION.\n");
                break;
            case OXL_NET_RX_USER_RENAME:
                fprintf(stderr, "--- DEBUG: Read message USER_RENAME.\n");
                break;
            case OXL_NET_RX_USER_LEAVING:
                fprintf(stderr, "--- DEBUG: Read message USER_LEAVING.\n");
                break;
            case OXL_NET_RX_USER_EXIT_ROOM:
                fprintf(stderr, "--- DEBUG: Read message USER_EXIT_ROOM.\n");
                break;
            case OXL_NET_RX_PROP_MOVE:
                fprintf(stderr, "--- DEBUG: Read message PROP_MOVE.\n");
                break;
            case OXL_NET_RX_PROP_DELETE:
                fprintf(stderr, "--- DEBUG: Read message PROP_DELETE.\n");
                break;
            case OXL_NET_RX_PROP_NEW:
                fprintf(stderr, "--- DEBUG: Read message PROP_NEW.\n");
                break;
            case OXL_NET_RX_DOOR_LOCK:
                fprintf(stderr, "--- DEBUG: Read message DOOR_LOCK.\n");
                break;
            case OXL_NET_RX_DOOR_UNLOCK:
                fprintf(stderr, "--- DEBUG: Read message DOOR_UNLOCK.\n");
                break;
            case OXL_NET_RX_PICT_MOVE:
                fprintf(stderr, "--- DEBUG: Read message PICT_MOVE.\n");
                break;
            case OXL_NET_RX_SPOT_STATE:
                fprintf(stderr, "--- DEBUG: Read message SPOT_STATE.\n");
                break;
            case OXL_NET_RX_SPOT_MOVE:
                fprintf(stderr, "--- DEBUG: Read message SPOT_MOVE.\n");
                break;
            case OXL_NET_RX_DRAW_CMD:
                fprintf(stderr, "--- DEBUG: Read message DRAW_CMD.\n");
                break;
            case OXL_NET_RX_INCOMING_FILE:
                fprintf(stderr, "--- DEBUG: Read message INCOMING_FILE.\n");
                break;
            case OXL_NET_RX_NAV_ERROR:
                fprintf(stderr, "--- DEBUG: Read message NAV_ERROR.\n");
                break;
            case OXL_NET_RX_AUTHENTICATE:
                fprintf(stderr, "--- DEBUG: Read message AUTHENTICATE.\n");
                break;
            case OXL_NET_RX_BLOWTHRU:
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
            uv_close((uv_handle_t *)stream, oxl_client_do_conn_close_cb);
        }
    }

    fprintf(stderr, "--- DEBUG: oxl_client_read_cb free(buf->base)\n");
    if (buf->base) {
        free(buf->base);
    }
}

/* uv_connect_cb */
void oxl_client_do_conn_open_cb(uv_connect_t *conn, int status)
{
    fprintf(stderr, "--- DEBUG: oxl_client_conn_open_cb\n");
    oxl_client_t *self = conn->data;

    if (status) { /* error condition */
        fprintf(stderr,
                "--- DEBUG: error: %d. %s\n",
                status,
                uv_strerror(status));
        
        uv_close((uv_handle_t *)conn->handle, oxl_client_do_conn_close_cb);
    }
    else {
        fprintf(stderr, "--- DEBUG: connect successful.\n");
        self->state = STATE_HANDSHAKING;
        conn->handle->data = self;
        
        uv_read_start(conn->handle,
                      oxl_client_do_alloc_buffer,
                      oxl_client_do_read_cb);
    }
}

/* getaddrinfo_cb */ 
void oxl_client_do_resolve_cb(uv_getaddrinfo_t *req,
                              int status,
                              struct addrinfo *res)
{
    fprintf(stderr, "--- DEBUG: oxl_client_do_resolve_cb\n");
    oxl_client_t *self = (oxl_client_t *)req->data;

    if (-1 == status) {
        fprintf(stderr,
                "getaddrinfo callback error %s\n",
                uv_strerror(status));
    }
    else {
        fprintf(stderr, "--- DEBUG: resolve status OK.\n");

        char addr[OXL_INET_ADDR_SZ_CAP] = {'\0'};
        fprintf(stderr, "--- DEBUG: before uv_ip4_name\n");

        /* need to appropraitely handle failure here. */
        /* currently, this just segfaults if uv_ip4_name failes. */
        uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, res->ai_addrlen);
        fprintf(stderr, "--- DEBUG: resolved to: %s\n", addr);

        uv_connect_t *conn_req = malloc(sizeof *conn_req);
        uv_tcp_t *conn = malloc(sizeof *conn);

        /* conn->data = self; */
        uv_tcp_init(self->loop, conn);
        self->conn = conn;
        conn_req->data = self;
        uv_tcp_connect(conn_req,
                       conn,
                       (struct sockaddr *)res->ai_addr,
                       oxl_client_do_conn_open_cb);
    }
    uv_freeaddrinfo(res);
}

/* uv_write_cb */
void oxl_client_do_write_cb(uv_write_t *req, int status)
{
    write_req_t *wr = (write_req_t *)req;

    if (0 <= status) {
        fprintf(stderr, "--- DEBUG: wrote: %s\n", req->data);
    }

    free(wr->buf.base);
    free(wr);
}

void oxl_client_stop(oxl_client_t *self) {
    /* free memory here */
    /* close connection? */
    /* free(client->host); */
}

void oxl_client_start(oxl_client_t *self,
                      uv_loop_t *loop,
                      char *username,
                      char *wizpass,
                      char *host,
                      unsigned int port,
                      unsigned int initial_room_id)
{
    fprintf(stderr, "--- DEBUG: oxl_client_start\n");

    /* init */
    oxl_crypt_init(&self->crypt);
    self->loop = loop;
    self->is_puid_changed = 0;
    self->puid_counter = 0xf5dc385e;
    self->puid_crc = 0xc144c580;
    self->reg_counter = 0xcf07309c;
    self->reg_crc = 0x5905f923;

    /* start */
    size_t z_username = strlen(username) + 1;
    size_t z_wizpass = strlen(wizpass) + 1;

    /* client->username = calloc(z, OXL_NET_MAX_USERNAME_LEN); */
        /* sizeof *client->username); */
    memset(&self->username, '\0', OXL_CLIENT_USERNAME_SZ_CAP);
    strlcpy(self->username, username, z_username);
    memset(&self->wizpass, '\0', OXL_CLIENT_WIZ_PASS_SZ_CAP);
    strlcpy(self->wizpass, wizpass, z_wizpass);
    self->room_id = initial_room_id;

    char port_str[OXL_INET_PORT_SZ_CAP];
    snprintf(port_str, OXL_INET_PORT_SZ_CAP - 1, "%d", port);
    fprintf(stderr, "--- DEBUG: host: %s\n", host);
    fprintf(stderr, "--- DEBUG: port: %s\n", port_str);

    struct addrinfo *hints = malloc(sizeof *hints);
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;

    /* fprintf(stderr, "--- DEBUG: before uv_getaddrinfo\n"); */
    uv_getaddrinfo_t *resolver = malloc(sizeof *resolver);
    resolver->data = self;

    int r = uv_getaddrinfo(self->loop,
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


void oxl_client_say(oxl_client_t *self,
                    size_t size,
                    uv_buf_t buf,
                    uv_write_cb write_cb)
{
    fprintf(stderr, "--- DEBUG: oxl_client_say\n");
    fprintf(stderr, "--- DEBUG: received buf from app:\n");
    oxl_dump_buf(buf, size);

    write_req_t *wr = malloc(sizeof *wr);
    wr->req.data = self;

    oxl_net_say_bubble_t *bubble = malloc(sizeof *bubble);

    int z = sizeof(oxl_net_say_t);
    wr->buf = uv_buf_init(malloc(z), z);
    oxl_net_say_init((oxl_net_say_t *)wr->buf.base, self, buf.base);

    oxl_net_say_dump((oxl_net_say_t *)wr->buf.base);
    
    fprintf(stderr,
            "--- DEBUG: preparing to send the following ciphertext:\n");
    oxl_dump_buf(wr->buf, size);
    write_data((uv_stream_t *)self->conn, size, wr->buf, write_cb);
}

void oxl_client_set_username(oxl_client_t *client, char *username) {
    strlcpy(client->username, username, OXL_CLIENT_USERNAME_SZ_CAP);
}
