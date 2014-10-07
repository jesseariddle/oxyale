#pragma once
#ifndef OX_CLIENT_H_
#define OX_CLIENT_H_

#define        OX_INET_PORT_STR_LEN 5
#define        OX_INET_ADDR_STR_LEN 15
#define       OX_INET6_ADDR_STR_LEN 45
#define        OX_INET_DEFAULT_PORT 9998
#define          OX_NET_HEADER_SIZE 12

#define    AUXFLAGS_UNKNOWN_MACHINE 0
#define             AUXFLAGS_MAC68K 1
#define             AUXFLAGS_MACPPC 2
#define              AUXFLAGS_WIN16 3
#define              AUXFLAGS_WIN32 4
#define               AUXFLAGS_JAVA 5

#define             AUXFLAGS_OSMASK 0x0000000F
#define       AUXFLAGS_AUTHENTICATE 0x80000000

#define        ULCAPS_ASSETS_PALACE 0x00000001
#define           ULCAPS_ASSETS_FTP 0x00000002
#define          ULCAPS_ASSETS_HTTP 0x00000004
#define         ULCAPS_ASSETS_OTHER 0x00000008
#define         ULCAPS_FILES_PALACE 0x00000010
#define            ULCAPS_FILES_FTP 0x00000020
#define           ULCAPS_FILES_HTTP 0x00000040
#define          ULCAPS_FILES_OTHER 0x00000080
#define           ULCAPS_EXTEND_PKT 0x00000100

#define        DLCAPS_ASSETS_PALACE 0x00000001
#define           DLCAPS_ASSETS_FTP 0x00000002
#define          DLCAPS_ASSETS_HTTP 0x00000004
#define         DLCAPS_ASSETS_OTHER 0x00000008
#define         DLCAPS_FILES_PALACE 0x00000010
#define            DLCAPS_FILES_FTP 0x00000020
#define           DLCAPS_FILES_HTTP 0x00000040
#define          DLCAPS_FILES_OTHER 0x00000080
#define       DLCAPS_FILES_HTTPSRVR 0x00000100
#define           DLCAPS_EXTEND_PKT 0x00000200

#define OX_NET_HEADER_FIELDS                                                  \
  /* public */                                                                \
  int message_id;                                                             \
  int message_size;                                                           \
  int message_p;                                                              \

#define OX_NET_MESSAGE_FIELDS                                                 \
  /* public */                                                                \
  /* TODO this is where i left off */

typedef struct ox_message_s {
    OX_NET_HEADER_FIELDS
    OX_NET_MESSAGE_FIELDS
} ox_message_t;

/* don't export */
#undef OX_NET_HEADER_FIELDS

enum conn_state {
    c_busy,  /* Busy; waiting for incoming data or for a write to complete. */
    c_done,  /* Done; read incoming data or write finished. */
    c_stop,  /* Stopped. */
    c_dead
};

typedef enum {
    STATE_DISCONNECTED,
    STATE_HANDSHAKING,
    STATE_READY
} ox_client_connection_state_e;

typedef struct ox_client_s {
    /* uv_work_t req; */

    uv_loop_t *loop;

    struct addrinfo *ai;
    int sockfd;
    ox_client_connection_state_e state;

    int is_connected;
    
    char *host;
    unsigned int port;
    unsigned int room_id;
    char *username;

    int puid_changed;
    unsigned int puid_counter;
    unsigned int puid_crc;
    unsigned int reg_counter;
    unsigned int reg_crc;

    uv_getaddrinfo_t resolver;
    int message_id;
    int message_size;
    int message_p;
    int waiting_for_more;
} ox_client_t;

/* init client */
void ox_client_init(ox_client_t *client, uv_loop_t *loop);
/* start client */
void ox_client_start(ox_client_t *client, char *username, char *host, unsigned int port, unsigned int initial_room);
/* alloc_cb */
void ox_client_alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
/* getaddrinfo_cb */
void ox_client_on_resolve_complete(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
/* uv_connect_cb */
void ox_client_on_connect_complete(uv_connect_t *connection, int status);
/* uv_connect_cb? */
void ox_client_on_disconnect_complete(uv_handle_t *handle);
/* void ox_client_on_alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf); */
void ox_client_on_resolve_host_complete(const char *fqdn, const int port, uv_getaddrinfo_cb cb);
/* uv_read_cb */
void ox_client_on_read_complete(uv_stream_t *req, ssize_t nread, const uv_buf_t *buf);


#endif /* OX_CLIENT_H_ */
