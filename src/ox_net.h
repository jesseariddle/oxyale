#pragma once
#ifndef OX_CLIENT_H_
#define OX_CLIENT_H_

#define        OX_INET_PORT_STR_LEN 5
#define        OX_INET_ADDR_STR_LEN 15
#define       OX_INET6_ADDR_STR_LEN 45
#define                DEFAULT_PORT 9998

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
} ox_client_connection_state_e;

typedef struct {
    struct addrinfo *ai;
    int sockfd;
    ox_client_connection_state_e state;
} ox_client_t;

void ox_net_init(ox_client_t *client, struct addrinfo *sock);
int32_t ox_net_connect(ox_client_t *client, const char *username, const char *host, const uint16_t port);
void ox_net_close(ox_client_t *client);

void ox_net_on_connect(uv_connect_t *req, int status);
void ox_net_on_close();
void ox_net_on_write_end(uv_write_t *req, int status);
uv_buf_t ox_net_on_alloc_buffer(uv_handle_t *handle, size_t suggested_size);
void ox_net_on_echo_read(uv_stream_t *server, ssize_t nread, uv_buf_t buf);

void ox_net_resolve_host(char *hostname);

#endif /* OX_CLIENT_H_ */
