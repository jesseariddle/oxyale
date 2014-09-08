#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../libuv/include/uv.h"
#include "../ox_client.h"

uv_loop_t *loop;

int main(int argc, char *argv[])
{
    fprintf(stderr, "DEBUG: main\n");
    loop = uv_default_loop();
    ox_client_start("xyzzy", "otakucool.epalaces.com", OX_INET_DEFAULT_PORT, 0 /* initial room */);
    return uv_run(loop, UV_RUN_DEFAULT);
}
