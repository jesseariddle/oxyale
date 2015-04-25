#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <uv.h>
#include <oxyale.h>

#define STDIN_FILENO 0
#define STDOUT_FILENO 1

uv_loop_t *loop_;
uv_pipe_t stdin_pipe_;
uv_pipe_t stdout_pipe_;
oxl_client_t client_;

void alcazar_say_cb(uv_write_t *req, int status)
{
    write_req_t *wr = (write_req_t *)req;

    fprintf(stderr, "--- DEBUG: alcazar_say_cb\n");
    printf("%s: %s\n", client_.username, wr->buf.base);

    free(wr->buf.base);
    free(wr);
}

void stdout_write_complete(uv_write_t *req, int status)
{
    /* fprintf(stderr, "--- DEBUG: stdout_write_complete\n"); */
    free_write_req((write_req_t *)req);
}

void read_stdin(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
{
    fprintf(stderr, "--- DEBUG: read_stdin\n");

    if (nread < 0) {
        uv_close((uv_handle_t *)&stdin_pipe_, NULL);
        uv_close((uv_handle_t *)&stdout_pipe_, NULL);
    }
    else if (0 < nread) {
        /* write_data((uv_stream_t *)&stdout_pipe_, nread, *buf, stdout_write_complete); */
        printf("Type to talk. /quit to quit\r\n: ");
        if (buf->base[nread - 1] == '\n') {
            buf->base[nread - 1] = '\0';
        }
        /*
        fprintf(stderr,
                "--- DEBUG: read the following from stdin "
                "(after \\n -> \\0):\n");
        oxl_dump_buf(*buf, nread);
        */
        
        if (!strcmp(buf->base, "/quit")) {
            uv_read_stop((uv_stream_t *)&stdin_pipe_);
            uv_stop(loop_);
        }
        else if (!strncmp(strtok(buf->base, " "), "/gotoroom", 9)) {
            char *ch = strtok(NULL, " ");
            if (ch != NULL) {
                unsigned int room_id = atoi(ch);
                fprintf(stderr, "--- DEBUG: moving to room %d...\n", room_id);
                /* oxl_client_goto_room(&client_, room_id); */
                oxl_client_leave_room_sync(&client_);
                oxl_client_join_room(&client_, room_id);
            }
            strtok(NULL, " ");
        }
        else {
            /* encrypt and send */
            /* err, that is, send via client */
            fprintf(stderr, "--- DEBUG: trying to speak...\n");
            oxl_client_say(&client_, nread, *buf, alcazar_say_cb);
        }
    }

    /* fprintf(stderr, "--- DEBUG: read_stdin free(buf.base)\n"); */
    if (buf->base) {
        free(buf->base);
    }
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    /* fprintf(stderr, "--- DEBUG: alloc_buffer\n"); */
    *buf = uv_buf_init(malloc(suggested_size), suggested_size);
}

int main(int argc, char *argv[])
{
    fprintf(stderr, "--- DEBUG: main\n");
    char username[OXL_CLIENT_USERNAME_SZ_CAP];
    char wizpass[OXL_CLIENT_WIZ_PASS_SZ_CAP];
    char server_fqdn[OXL_CLIENT_FQDN_SZ_CAP];
    char port_s[OXL_CLIENT_PORT_SZ_CAP];
    long port;
    int room_id;

    loop_ = uv_default_loop();

    /* oxl_client_t *client = malloc(sizeof *client); */
    /* oxl_client_init(&client_, loop_); */

    printf("username [xyzzy-oxyale]: ");
    fgets(username, OXL_CLIENT_USERNAME_SZ_CAP, stdin);
    if (0 < strlen(username) && username[0] == '\n') {
        strcpy(username, "xyzzy-oxyale"); /* %31s%n */
    }

    /* printf("server [otakucool.epalaces.com]: "); */
    /* printf("server [pixelcube.epalaces.com]: "); */
    printf("server [thexpalace.com]: ");
    fgets(server_fqdn, OXL_CLIENT_FQDN_SZ_CAP, stdin);
    if (0 < strlen(server_fqdn) && server_fqdn[0] == '\n') {
        /* strcpy(server_fqdn, "otakucool.epalaces.com"); */ /* %255s%n */
        strcpy(server_fqdn, "thexpalace.com");
    }

    printf("wiz pass []: ");
    fgets(wizpass, OXL_CLIENT_WIZ_PASS_SZ_CAP, stdin);
    if (0 < strlen(wizpass) && wizpass[0] == '\n') strcpy(wizpass, "");

    printf("port [%d]: ", OXL_INET_DEFAULT_PORT);
    fgets(port_s, OXL_CLIENT_PORT_SZ_CAP, stdin);
    if (0 < strlen(port_s) && port_s[0] == '\n') port = OXL_INET_DEFAULT_PORT;
    else port = strtol(port_s, (char **)NULL, 10);

    room_id = 0;
    oxl_client_start(&client_,
                     loop_,
                     username,
                     wizpass,
                     server_fqdn,
                     port,
                     room_id /* initial room */);

    /* client_.cb->client_conn_start_cb = do_something; */
    
    fprintf(stderr, "--- DEBUG: opening stdin pipe\n");
    uv_pipe_init(loop_, &stdin_pipe_, 0);
    uv_pipe_open(&stdin_pipe_, STDIN_FILENO);

    fprintf(stderr, "--- DEBUG: opening stdout pipe\n");
    uv_pipe_init(loop_, &stdout_pipe_, 0);
    uv_pipe_open(&stdout_pipe_, STDOUT_FILENO);

    fprintf(stderr, "--- DEBUG: performing read on stdin pipe\n");
    uv_read_start((uv_stream_t *)&stdin_pipe_, alloc_buffer, read_stdin);

    fprintf(stdout, "Type to talk. /quit to quit\n");
    int result = uv_run(loop_, UV_RUN_DEFAULT);

    fprintf(stderr, "--- DEBUG: stopping client\n");
    oxl_client_stop(&client_);

    return result;
}
