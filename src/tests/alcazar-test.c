#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../libuv/include/uv.h"
#include "../ox_client.h"

uv_loop_t *loop;

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

/* uv_alloc_cb */
void alloc_cb(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    fprintf(stderr, "DEBUG: alloc_cb\n");
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

/* uv_connect_cb */
void connect_cb(uv_connect_t *req, int status)
{
    fprintf(stderr, "DEBUG: connect_cb\n");
    if (status) {
        fprintf(stderr, "Error: %d. %s\n", status, uv_strerror(status));
    }
    else {
         printf("Feelin' fine.\n");
    }
}

/* getaddrinfo_cb */
void resolve_cb(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    fprintf(stderr, "DEBUG: resolve_cb\n");
    if (-1 == status) {
        fprintf(stderr, "getaddrinfo callback error %s\n", uv_strerror(status));
        return;
    }
    fprintf(stderr, "DEBUG: resolve... successful.\n");
    char addr[16] = {'\0'};
    uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, OX_INET_ADDR_STR_LEN);
    fprintf(stderr, "DEBUG: Resolved to: %s\n", addr);
    uv_connect_t *connect_req = malloc(sizeof *connect_req);
    uv_tcp_t *socket = malloc(sizeof *socket);
    uv_tcp_init(loop, socket);
    connect_req->data = (void *)socket;
    uv_tcp_connect(connect_req, socket, (const struct sockaddr *)&res->ai_addr, connect_cb);
    uv_freeaddrinfo(res);
}

void resolve_host(const char *fqdn, const int port, uv_getaddrinfo_cb cb)
{
    fprintf(stderr, "DEBUG: ox_net_resolve_host\n");
    char port_str[OX_INET_PORT_STR_LEN + 1];
    snprintf(port_str, OX_INET_PORT_STR_LEN, "%d", port);
    fprintf(stderr, "DEBUG: host: %s\n", fqdn);
    fprintf(stderr, "DEBUG: port: %s\n", port_str);
    uv_loop_t *loop = uv_default_loop();
    struct addrinfo *hints = malloc(sizeof *hints);;
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;
    fprintf(stderr, "DEBUG: before uv_getaddrinfo\n");
    uv_getaddrinfo_t *resolver = malloc(sizeof *resolver);
    int r = uv_getaddrinfo(loop, resolver, cb, fqdn, port_str, hints);
    fprintf(stderr, "DEBUG: after uv_getaddrinfo\n");
    if (r) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_strerror(r));
        return;
    }
}

int main(int argc, char *argv[])
{
    fprintf(stderr, "DEBUG: main\n");
    loop = uv_default_loop();
    ox_net_start("otakucool.epalaces.com", OX_INET_DEFAULT_PORT, 0 /* initial room */);
    return uv_run(loop, UV_RUN_DEFAULT);
}
