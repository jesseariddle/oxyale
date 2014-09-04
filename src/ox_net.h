#pragma once
#ifndef OX_NET_H_
#define OX_NET_H_

#define        OX_INET_PORT_STR_LEN 5
#define        OX_INET_ADDR_STR_LEN 15
#define       OX_INET6_ADDR_STR_LEN 45
#define         OX_NET_DEFAULT_PORT 9998

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

typedef enum {
    STATE_DISCONNECTED,
    STATE_HANDSHAKING,
    STATE_READY
} ox_net_connection_state_e;

typedef struct {
    struct addrinfo *ai;
    int sockfd;
    ox_net_connection_state_e state;
} ox_net_t;

typedef struct ox_net_connect_s {
    uv_work_t req;
    char *host;
    unsigned int port;
    unsigned int initial_room;
    char *username;
} ox_net_connect_t;

void ox_net_connect_to_host(char *host, unsigned int port, unsigned int initial_room);

/* uv_connect_cb */
void ox_net_on_connect(uv_connect_t *req, int status);
/* getaddrinfo_cb */
void ox_net_on_resolve(uv_getaddrinfo_t *req, int status, struct addrinfo *res);
/* uv_connect_cb */
/* void ox_net_connect(ox_net_t *client, const char *username, const char *host, const uint16_t port, uv_connect_cb cb); */
void ox_net_on_close(ox_net_t *client);
void ox_net_on_alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void ox_net_on_resolve_host(const char *fqdn, const int port, uv_getaddrinfo_cb cb);

#endif /* OX_NET_H_ */
