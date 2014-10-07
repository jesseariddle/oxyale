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
#include "ox_net.h"
#include "ox_client.h"

/* static uv_getaddrinfo_t resolver_; */
/* static int message_id_; */
/* static int message_size_; */
/* static int message_p_; */
/* static int waiting_for_more_; */

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
ox_client_deinit() {
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

    ox_net_start(username, host, port);
}

/* getaddrinfo_cb */ 
void 
ox_net_on_resolve(uv_getaddrinfo_t *req, int status, struct addrinfo *res) {
    fprintf(stderr, "--- DEBUG: ox_net_on_resolve\n");

    if (-1 == status) {
        fprintf(stderr, "getaddrinfo callback error %s\n", uv_strerror(status));
    }
    else {
        fprintf(stderr, "--- DEBUG: resolve... successful.\n");
        char addr[16] = {'\0'};
        uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, OX_INET_ADDR_STR_LEN);
        fprintf(stderr, "--- DEBUG: Resolved to: %s\n", addr);
        uv_connect_t *connect_req = malloc(sizeof *connect_req);
        uv_tcp_t *socket = malloc(sizeof *socket);
        uv_loop_t *loop = uv_default_loop();
        uv_tcp_init(loop, socket);
        connect_req->data = socket;
        uv_tcp_connect(connect_req, socket, (struct sockaddr *)res->ai_addr, ox_net_on_connect);
    }
    uv_freeaddrinfo(res);
    free(req->data);
}

/* uv_connect_cb */
void 
ox_net_on_connect(uv_connect_t *connection, int status) {
    fprintf(stderr, "--- DEBUG: ox_net_on_connect\n");

    if (status) {
        fprintf(stderr, "--- DEBUG: Error: %d. %s\n", status, uv_strerror(status));
        uv_close(connection->handle, ox_net_on_close);
    }
    else {
        fprintf(stderr, "--- DEBUG: connect successful.\n");
        uv_read_start(connection->handle, ox_net_alloc_buffer, ox_net_on_read);
    }
}

/* void ox_client_connect_cb(uv_connect_t *req, int status) { */
/*     int NUM_WRITE_REQS = 4; */
/*     uv_write_t *wr = (uv_write_t *)req; */
/*     uv_buf_t buf; */
/*     int i, r; */
/*     buf = uv_buf_init("PING", 4); */
/*     for (i = 0; i < NUM_WRITE_REQS; ++i) { */
/*     	req = malloc(sizeof *req); */
/*     	assert(req != NULL); */
/*     	//r = uv_write(req, (uv_stream_t*)&tcp_handle, &buf, 1, write_cb)); */
/*     	assert(r == 0); */
/*     } */
/*     //uv_close((uv_handle_t*)&tcp_handle, ox_client_close_cb); */
/* } */

/*
void ox_client_on_close()
{
    close(client->sockfd);
    freeaddrinfo(client->ai);
}
*/

/* uv_read_cb */
void 
ox_net_on_read(uv_stream_t *req, ssize_t nread, const uv_buf_t *buf) {
    fprintf(stderr, "--- DEBUG: ox_net_on_read\n");
    if (0 < nread) {
        fprintf(stderr, "--- DEBUG: successfully read %d bytes of data:\n", nread);
        fprintf(stderr, "--- DEBUG: read: %s\n", buf->base);
    }
    else {
        fprintf(stderr, "--- DEBUG: error: closing connection\n");
        uv_close((uv_handle_t *)req, ox_net_on_close);
    }
    fprintf(stderr, "--- DEBUG: freeing req->data\n");
    free(req->data);
}

/* uv_write_cb */
void 
ox_net_on_write(uv_write_t *req, int status) {
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
ox_client_on_close(uv_handle_t *handle) {
    fprintf(stderr, "--- DEBUG: ox_net_on_close()\n");
}
