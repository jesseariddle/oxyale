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

static uv_getaddrinfo_t resolver;
static int message_id;
static int message_size;
static int message_p;
static int waiting_for_more;

void ox_net_start(char *host, unsigned int port, unsigned int initial_room))
{
    fprintf(stderr, "DEBUG: ox_net_resolve_host\n");
    char port_str[OX_INET_PORT_STR_LEN + 1];
    snprintf(port_str, OX_INET_PORT_STR_LEN, "%d", port);
    fprintf(stderr, "DEBUG: host: %s\n", fqdn);
    fprintf(stderr, "DEBUG: port: %s\n", port_str);
    uv_loop_t *loop = uv_default_loop();

    struct addrinfo *hints = malloc(sizeof *hints);
    hints->ai_family = PF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = 0;

    fprintf(stderr, "DEBUG: before uv_getaddrinfo\n");
    uv_getaddrinfo_t *resolver = malloc(sizeof *resolver);
    int r = uv_getaddrinfo(loop, resolver, ox_net_on_resolve, fqdn, port_str, hints);
    fprintf(stderr, "DEBUG: after uv_getaddrinfo\n");
    if (r) {
        fprintf(stderr, "getaddrinfo call error %s\n", uv_strerror(r));
        return;
    }
}

/* getaddrinfo_cb */ 
void ox_net_on_resolve(uv_getaddrinfo_t *req, int status, struct addrinfo *res)
{
    fprintf(stderr, "DEBUG: ox_net_on_resolve\n");
    if (-1 == status) {
        fprintf(stderr, "getaddrinfo callback error %s\n", uv_strerror(status));
    }
    else {
        fprintf(stderr, "DEBUG: resolve... successful.\n");
        char addr[16] = {'\0'};
        uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, OX_INET_ADDR_STR_LEN);
        fprintf(stderr, "DEBUG: Resolved to: %s\n", addr);
        uv_connect_t *connect_req = malloc(sizeof *connect_req);
        uv_tcp_t *socket = malloc(sizeof *socket);
        uv_loop_t *loop = uv_default_loop();
        uv_tcp_init(loop, socket);
        connect_req->data = socket;
        uv_tcp_connect(connect_req, socket, (struct sockaddr *)res->ai_addr, ox_net_on_connect);
    }
    uv_freeaddrinfo(res);
    free(req->data);
    free(req);
}

/* uv_connect_cb */
void ox_net_on_connect(uv_connect_t *req, int status)
{
    fprintf(stderr, "DEBUG: ox_net_on_connect\n");
    if (status) {
        fprintf(stderr, "Error: %d. %s\n", status, uv_strerror(status));
    }
    else {
        fprintf("DEBUG: connect successful.\n");
    }
    free(req->data);
    free(req);
}

/* void ox_net_connect_cb(uv_connect_t *req, int status) */
/* { */
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
/*     //uv_close((uv_handle_t*)&tcp_handle, ox_net_close_cb); */
/* } */

void ox_net_on_close()
{
    close(client->sockfd);
    freeaddrinfo(client->ai);
}

/* uv_write_cb */
void ox_net_write_start()
{
    
}

/* uv_alloc_cb */
void ox_net_on_alloc_buffer(uv_handle_t *handle, int suggested_size, uv_buf_t *buf)
{
    *buf = malloc(suggested_size);
}
