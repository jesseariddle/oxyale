/*
 * File: ox_net.c
 * Description: This file performs low level communication. It passes raw data to and from the client layer.
 */

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

/* uv_alloc_cb */
void 
ox_net_alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

void
ox_net_init(ox_net_t *net, 
            ox_client_connect_complete_cb *ox_client_on_connect_complete, 
            ox_client_disconnect_complete_cb *ox_client_on_disconnect_complete, 
            ox_client_on_read_complete_cb *ox_client_on_read_complete, 
            ox_client_on_write_complete_cb *ox_client_on_write_complete, 
            uv_loop_t *loop) {
    net->loop = loop;
    net->ox_client_on_connect_complete = ox_client_on_connect_complete;
    net->ox_client_on_disconnect_complete = ox_client_on_disconnect_complete;
    net->ox_client_on_read_complete = ox_client_on_read_complete;
    net->ox_client_on_write_complete = ox_client_on_write_complete;
}

void 
ox_net_start(char *username, char *host, unsigned int port) {
    fprintf(stderr, "--- DEBUG: ox_net_start\n");

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
    int r = uv_getaddrinfo(loop_, resolver, ox_net_on_resolve_complete, host, port_str, hints);
    fprintf(stderr, "--- DEBUG: after uv_getaddrinfo\n");
    if (r) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_strerror(r));
        return;
    }
    net->ox_client_on_connect();
}

/* getaddrinfo_cb */ 
void 
ox_net_on_resolve_complete(uv_getaddrinfo_t *req, int status, struct addrinfo *res) {
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
        uv_tcp_init(loop_, socket);
        connect_req->data = socket;
        uv_tcp_connect(connect_req, socket, (struct sockaddr *)res->ai_addr, ox_net_on_connect);
    }
    uv_freeaddrinfo(res);
    free(req->data);
    free(req);
}

/* uv_connect_cb */
void 
ox_net_on_connect_complete(uv_connect_t *connection, int status) {
    fprintf(stderr, "--- DEBUG: ox_net_on_connect\n");
    if (status) {
        fprintf(stderr, "--- DEBUG: Error: %d. %s\n", status, uv_strerror(status));
        uv_close(connection, ox_net_on_close);
    }
    else {
        fprintf(stderr, "--- DEBUG: connect successful.\n");
        /* uv_tcp_t *client = malloc(sizeof *client); */
        uv_tcp_t *client = (uv_tcp_t *)connection->handle;
        uv_read_start((uv_stream_t *)client, ox_net_alloc_buffer, ox_net_on_read);
    }
    /* free(connection->data); */ /* uv_socket_t* */
    /* free(connection); */ /* uv_connect_t* */
}

/* uv_read_cb */
void 
ox_net_on_read_complete(uv_stream_t *req, ssize_t nread, const uv_buf_t *buf) {
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
    /* call client callback */
    free(req->data);
}

/* uv_write_cb */
void 
ox_net_on_write_complete(uv_write_t *req, int status) {
    if (0 < req->data) {
        fprintf(stderr, "--- DEBUG: wrote: %s\n", req->data);
    }
    free(req->data);
}

void 
ox_net_on_connection_close_complete(uv_handle_t *connection) {
    fprintf(stderr, "--- DEBUG: ox_net_on_close\n");
    if (connection->data)
        free(connection->data);
    if (connection)
        free(connection);
}
