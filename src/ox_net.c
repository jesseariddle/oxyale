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

#include "libuv/include/uv.h"
#include "ox_net.h"

void ox_net_init(ox_client_t *client, struct addrinfo *ai)
{
    memset(ai, 0, sizeof *ai);
    ai->ai_flags = AI_PASSIVE;
    ai->ai_family = AF_UNSPEC;
    ai->ai_socktype = SOCK_STREAM;
    /* sock->ai_protocol = 1; */

    client->ai = ai;
    client->sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
}

/* int ox_resolve returns
 * 0: if success
 * != 0: if error
 */
static int ox_net_resolve(struct addrinfo *ai, const char *fqdn, int port)
{
	char port_str[6];
	int c, i, z = sizeof port_str;
	for (i = port; 0 < i; c = port[i] % 10, port /= 10) {
		port_str[z - i - 1] = c;
	}

	loop = uv_default_loop();
	struct addrinfo hints;
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = 0;

	uv_getaddrinfo_t resolver;
	int r = uv_getaddrinfo(loop, &resolver, ox_net_resolve_cb, fqdn, ox_int2str(port), &hints);
	if (r) {
		fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(uv_last_err(loop)));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}

static int ox_net_resolve_cb()
{

}

/* int ox_client_connect returns
 * -1: cannot connect
 * -2: cannot resolve service
 * -3: cannot resolve host
 * 0: connected successfully
 */
int32_t ox_net_connect( ox_client_t *client, const char *username, const char *host, const uint16_t port )
{
    struct addrinfo *ai = client->ai;
    int8_t is_connected = 0;
    int32_t result;
    char *port_str = alloca(OX_INET_PORT_STR_LEN + 1);

    ox_strify( port_str, port );
    if ( 0 == ox_resolve( ai, host, port_str ) ) {
        ai->ai_addrlen = sizeof *ai->ai_addr;
        is_connected = connect( client->sockfd, ( struct sockaddr * )ai->ai_addr, ai->ai_addrlen );
        if ( !is_connected ) {
            close ( client->sockfd );
            result = -1;
        }
        result = 0;
    } else if ( 0 == ox_resolve( ai, host, NULL ) ) {
        /* can resolve host, but cannot connect to service */
        result = -2;
    } else {
        /* cannot resolve host */
        result = -3;
    }
    return result;
}

void ox_net_connect_cb(uv_connect_t *req, int status)
{
	int NUM_WRITE_REQS = 4;
    uv_write_t *req;
    uv_buf_t buf;
    int i, r;

    buf = uv_buf_init("PING", 4);
    for (i = 0; i < NUM_WRITE_REQS; ++i) {
    	req = malloc(sizeof *req);
    	ASSERT(req != NULL);
    	//r = uv_write(req, (uv_stream_t*)&tcp_handle, &buf, 1, write_cb));
    	ASSERT(r == 0);
    }

    //uv_close((uv_handle_t*)&tcp_handle, ox_net_close_cb);
}

void ox_net_close_cb()
{
    //close(client->sockfd);
    //freeaddrinfo(client->ai);
}
