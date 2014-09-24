#include "env.h"

#include "libuv/include/uv.h"
#include "ox_client.h"

void ox_client_init(ox_client_t *client, struct addrinfo *ai)
{
	uv_loop_t *loop = uv_default_loop();
	uv_tcp_init(loop, socket);
}

void ox_client_connect(ox_client_t *client, const char *username, const char *host, const uint16_t port)
{
    char *ipv4addr = ox_net_resolve_host(host);

    uv_connect_t *connect = malloc(sizeof uv_connect_t);
    struct sockaddr_in dest = uv_ipv4_addr("127.0.0.1", 80);
    uv_tcp_connect(connect, socket, dest, ox_client_on_connect);
}

void ox_client_on_connect(uv_stream_t *client, int status)
{
    
}

void ox_client_disconnect(ox_client_t *client)
{

}

void ox_client_on_disconnect(uv_stream_t *client, int status)
{

}
