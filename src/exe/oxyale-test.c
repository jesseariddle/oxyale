#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../libuv/include/uv.h"
#include "../ox_client.h"

uv_loop_t *loop_;

int main(int argc, char *argv[])
{
    fprintf(stderr, "DEBUG: main\n");
    loop_ = uv_default_loop;
    ox_client_init(loop_);
    ox_client_start("xyzzy", "otakucool.epalaces.com", OX_INET_DEFAULT_PORT, 0 /* initial room */);
    return uv_run(loop_, UV_RUN_DEFAULT);
}
