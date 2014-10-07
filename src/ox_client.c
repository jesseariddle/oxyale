#include <stdlib.h>
#include <assert.h>

#include "env.h"

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

#include "../deps/libuv/include/uv.h"
#include "ox_client.h"

void 
ox_client_init(ox_client_t *client, uv_loop_t *loop) {
    client->loop = loop;
    client->puid_changed = 0;
    client->puid_counter = 0xf5dc385e;
    client->puid_crc = 0xc144c580;
    client->reg_counter = 0xcf07309c;
    client->reg_crc = 0x5905f923;
}

void
ox_client_deinit(ox_client_t *client) {
    if (client->username) {
        free(client->username);
    }
}

void 
ox_client_start(ox_client_t *client, char *username, char *host, unsigned int port, unsigned int initial_room) {
    fprintf(stderr, "--- DEBUG: ox_client_start\n");

    size_t z = strlen(username);
    client->username = calloc(z + 1, sizeof *client->username);
    strlcpy(client->username, username, z);
    client->room_id = initial_room;

    char port_str[OX_INET_PORT_STR_LEN + 1];
    snprintf(port_str, OX_INET_PORT_STR_LEN, "%d", port);
    fprintf(stderr, "--- DEBUG: host: %s\n", host);
    fprintf(stderr, "--- DEBUG: port: %s\n", port_str);

    struct addrinfo *hints = malloc(sizeof *hints);
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;

    fprintf(stderr, "--- DEBUG: before uv_getaddrinfo\n");
    uv_getaddrinfo_t *resolver = malloc(sizeof *resolver);
    int r = uv_getaddrinfo(client->loop, resolver, ox_client_on_resolve_complete, host, port_str, hints);
    fprintf(stderr, "--- DEBUG: after uv_getaddrinfo\n");
    if (r) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_strerror(r));
        return;
    }
}

/* getaddrinfo_cb */ 
void 
ox_client_on_resolve_complete(uv_getaddrinfo_t *req, int status, struct addrinfo *res) {
    fprintf(stderr, "--- DEBUG: ox_client_on_resolve_complete\n");

    if (-1 == status) {
        fprintf(stderr, "getaddrinfo callback error %s\n", uv_strerror(status));
    }
    else {
        fprintf(stderr, "--- DEBUG: resolve... successful.\n");
        char addr[OX_INET_ADDR_STR_LEN + 1] = {'\0'};
        uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, OX_INET_ADDR_STR_LEN);
        fprintf(stderr, "--- DEBUG: resolved to: %s\n", addr);
        uv_connect_t *connect_req = malloc(sizeof *connect_req);
        uv_tcp_t *socket = malloc(sizeof *socket);
        uv_loop_t *loop = uv_default_loop();
        uv_tcp_init(loop, socket);
        connect_req->data = socket;
        uv_tcp_connect(connect_req, socket, (struct sockaddr *)res->ai_addr, ox_client_on_connect_complete);
    }
    uv_freeaddrinfo(res);
    free(req->data);
}

/* uv_connect_cb */
void 
ox_client_on_connect_complete(uv_connect_t *connection, int status) {
    fprintf(stderr, "--- DEBUG: ox_client_on_connect_complete\n");

    if (status) {
        fprintf(stderr, "--- DEBUG: error: %d. %s\n", status, uv_strerror(status));
        uv_close((uv_handle_t *)connection->handle, ox_client_on_disconnect_complete);
    }
    else {
        fprintf(stderr, "--- DEBUG: connect successful.\n");
        uv_read_start(connection->handle, ox_client_alloc_buffer, ox_client_on_read_complete);
    }
}

/* uv_read_cb */
void 
ox_client_on_read_complete(uv_stream_t *req, ssize_t nread, const uv_buf_t *buf) {
    fprintf(stderr, "--- DEBUG: ox_client_on_read\n");
    if (0 < nread) {
        /* we need to collect payloads then route the message (on a worker thread?) */
        /* we need to do this depending on the state of the connection */
        fprintf(stderr, "--- DEBUG: successfully read %ld bytes of data\n", nread);
        fprintf(stderr, "--- DEBUG: read: %s\n", buf->base);

        if (client->state == STATE_HANDSHAKING) {
            ox_client_do_handshake(req, nread, buf);
        }
        else if (client->state == STATE_READY) {
            if (message_id == 0) {
                if (OX_NET_HEADER_SIZE <= nread) {
                    message_id = ntohl(*((long *)buf->base));
                    message_size = ntohl(*((long *)buf->base + sizeof message_id));
                    ox_net_header_t *header = malloc(nsize);
                    memcpy(header, buf->base, nread);
                }
            }
        }
    }
    else {
        fprintf(stderr, "--- DEBUG: error: closing connection\n");
        uv_close((uv_handle_t *)req, ox_client_on_disconnect_complete);
    }
    fprintf(stderr, "--- DEBUG: freeing req->data\n");
    free(req->data);
}

/* uv_write_cb */
void 
ox_client_on_write_complete(uv_write_t *req, int status) {
    if (0 < req->data) {
        fprintf(stderr, "--- DEBUG: read: %s\n", req->data);
    }
    free(req->data);
}

/* uv_alloc_cb */
void 
ox_client_alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

void 
ox_client_on_disconnect_complete(uv_handle_t *handle) {
    fprintf(stderr, "--- DEBUG: ox_client_on_disconnect_complete()\n");
}

void 
ox_client_do_handshake(uv_read_t *req, ssize_t nread, uv_buf_t *buf) {
    int message_id;
    size_t size;
    int p;
    fprintf(stderr, "--- DEBUG: ox_client_handshake\n");
    
    message_id = req->data
}
