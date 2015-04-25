#pragma once
#ifndef OXYALE_H
#define OXYALE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <uv.h>
#include <defs.h>

#ifndef __cplusplus
#define false 0
#define true 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;

#define OXL_NET_HEADER_FIELDS \
/* public */ \
unsigned int msg_id; \
unsigned int msg_len; \
unsigned int msg_ref; \

extern uint8_t oxl_env_is_big_endian();
extern uint32_t oxl_uint32_swap_endian(const uint32_t n);
extern void oxl_strify(char *str, const uint32_t uint32);

typedef struct write_req_s {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

typedef struct oxl_prop_s {
    /* stub */
} oxl_prop_t;

typedef struct oxl_prop_store_s {
    
} oxl_prop_store_t;

typedef struct oxl_crypt_s {
    int32_t seed;
    byte lut[OXL_CRYPT_LUT_LEN];
} oxl_crypt_t;

typedef struct {
    
} oxl_net_say_bubble_t;

typedef void (*oxl_client_conn_open_start_cb)(void *data);
typedef void (*oxl_client_conn_open_complete_cb)(void *data);
typedef void (*oxl_client_conn_failed_cb)(void *data);
typedef void (*oxl_client_conn_close_cb)(void *data);
typedef void (*oxl_client_goto_url_cb)(void *data);
typedef void (*oxl_client_room_changed_cb)(void *data);
typedef void (*oxl_client_auth_req_cb)(void *data);
typedef void (*oxl_client_security_err_cb)(void *data);
/*
typedef void (*oxl_client_logon_cb)(void *data);
typedef void (*oxl_client_gotoroom_cb)(void *data);
typedef void (*oxl_client_say_cb)(void *data);
*/

typedef struct oxl_callbacks_s {
    oxl_client_conn_open_start_cb *client_conn_start_cb;
    oxl_client_conn_open_complete_cb *client_conn_complete_cb;
    oxl_client_conn_failed_cb *client_conn_failed_cb;
    oxl_client_conn_close_cb *client_conn_close_cb;
    oxl_client_goto_url_cb *client_goto_url_cb;
    oxl_client_room_changed_cb *client_room_changed_cb;
    oxl_client_auth_req_cb *client_auth_req_cb;
    oxl_client_security_err_cb *client_security_err_cb;
    /*
    oxl_client_logon_cb *client_logon_cb;
    oxl_client_goto_room_cb *client_goto_room_cb;
    oxl_client_say_cb *client_say_cb;
    */
} oxl_callbacks_t;

typedef struct oxl_user_s {
    OXL_NET_HEADER_FIELDS
    int id;
    char name[32];
    int x;
    int y;
    int room_id;
    char room_name[256];
    int prop_count;
    int face;
    int color;
    int flags;
    /* int prop_ids[256]; */
    /* int prop_crcs[256]; */
    /* oxl_client_prop_t props[256]; */
    void *prop_ids;
    void *prop_crcs;
    void *props;
    int should_show_face;
    oxl_prop_store_t prop_store;
} oxl_user_t;

typedef struct oxl_view_s {
    /* stub */
} oxl_view_t;

typedef struct oxl_hotspot_s {
    /* stub */
} oxl_hotspot_t;

typedef struct oxl_room_s {
    int id;
    char *name;
    char *background_filename;
    oxl_user_t *users;
    void *users_hash;
    int room_flags;
    void *spot_images;
    oxl_hotspot_t *hotspots;
    oxl_hotspot_t *hotspots_above_nothing;
    oxl_hotspot_t *hotspots_above_avatars;
    oxl_hotspot_t *hotspots_about_nametags;
    oxl_hotspot_t *hotspots_above_everything;
    void *hotspots_by_id;
    oxl_prop_t *loose_props;
    void *draw_front_commands;
    void *draw_back_commands;
    void *draw_layer_history;
    oxl_user_t *selected_user;
    int user_id;
    oxl_view_t *room_view;
    double dim_level;
    int should_show_avatars;
    char *chat_log;
    char *last_message;
    int last_message_count;
    int last_message_received;
    int last_message_timer;
    char *status_message;
    int status_disappear_timer;
} oxl_room_t;

typedef enum oxl_client_conn_state_e {
    STATE_DISCONNECTED,
    STATE_HANDSHAKING,
    STATE_CONNECTED /* STATE_READY */
} oxl_client_conn_state_e;

typedef struct oxl_client_s {
    OXL_NET_HEADER_FIELDS
    oxl_crypt_t crypt;
    uv_tcp_t *conn;
    uv_loop_t *loop;
    /* uv_getaddrinfo_t resolver; */
    struct addrinfo *ai;
    int sockfd;
    oxl_client_conn_state_e state;
    char *host;
    unsigned int port;
    unsigned int room_id;
    char server_name[OXL_CLIENT_SERVER_NAME_SZ_CAP];
    char username[OXL_CLIENT_USERNAME_SZ_CAP];
    char wizpass[OXL_CLIENT_WIZ_PASS_SZ_CAP];
    int is_puid_changed;
    unsigned int puid_counter;
    unsigned int puid_crc;
    unsigned int reg_counter;
    unsigned int reg_crc;
    int is_server_big_endian;
    int waiting_for_more;
    oxl_room_t room;
    int server_version;
    int permissions;
    oxl_prop_store_t prop_store;
    oxl_user_t user;
    oxl_callbacks_t cb;
} oxl_client_t;

void free_write_req(write_req_t *req);
void write_data(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback);

/* Regcode Section */
typedef struct oxl_regcode_s {
    uint32_t counter;
    uint32_t crc;
} oxl_regcode_t;

void oxl_regcode_from_str(oxl_regcode_t *self, const char *str);
void oxl_regcode_generate(oxl_regcode_t *self);
void oxl_regcode_print(const oxl_regcode_t *self);
void oxl_regcode_str_trim(char *trimmed_regcode_str, const char *str);
int32_t oxl_regcode_str_trim_len(const char *str);

void oxl_crypt_init(oxl_crypt_t *self);
void oxl_crypt_encrypt(const oxl_crypt_t crypt, const char pt, char *ct);
void oxl_crypt_decrypt(const oxl_crypt_t crypt, const char ct, char *pt);

typedef struct oxl_net_say_s {
    OXL_NET_HEADER_FIELDS
    ushort len;
    char msg[OXL_CRYPT_CIPHERTEXT_SZ_CAP];
} oxl_net_say_t;

void oxl_net_say_init(oxl_net_say_t *self, oxl_client_t *client, char *text);
void oxl_net_say_dump(oxl_net_say_t *self);

enum conn_state {
    c_busy,  /* Busy; waiting for incoming data or for a write to complete. */
    c_done,  /* Done; read incoming data or write finished. */
    c_stop,  /* Stopped. */
    c_dead
};

typedef struct oxl_net_header_s {
    OXL_NET_HEADER_FIELDS
} oxl_net_header_t;

/* typedef struct __attribute__((packed)) oxl_net_logon_s { */
typedef struct oxl_net_logon_s {
    OXL_NET_HEADER_FIELDS
    unsigned int reg_crc;
    unsigned int reg_counter;
    unsigned char username_len;
    char username[OXL_CLIENT_USERNAME_SZ_CAP];
    char wizpass[OXL_CLIENT_WIZ_PASS_SZ_CAP];
    unsigned int flags;
    unsigned int puid_counter;
    unsigned int puid_crc;
    unsigned int demo_elapsed;
    unsigned int total_elapsed;
    unsigned int demo_limit;
    short initial_room_id;
    char reserved[OXL_CLIENT_RESERVED_LOGON_LEN];
    unsigned int ul_upload_requested_protocol_version;
    unsigned int ul_upload_capabilities;
    unsigned int ul_download_capabilities;
    unsigned int ul_2d_engine_capabilities;
    unsigned int ul_2d_graphics_capabilities;
    unsigned int ul_3d_engine_capabilities;
} oxl_net_logon_t;

typedef struct oxl_server_info_s {
    OXL_NET_HEADER_FIELDS
    int permissions;
    unsigned char size;
    char server_name[256];
} oxl_server_info_t;

void oxl_net_logon_init(oxl_net_logon_t *self,
                        char *username,
                        char *wizpass,
                        int initial_room_id,
                        unsigned int reg_crc,
                        int reg_counter,
                        unsigned int puid_crc,
                        int puid_counter);

void oxl_net_logon_dump(const oxl_net_logon_t *self);

/* init client */
/* void oxl_client_init(oxl_client_t *self, uv_loop_t *loop); */
/* start client */
void oxl_client_start(oxl_client_t *self,
                      uv_loop_t *loop,
                      char *username,
                      char *wizpass,
                      char *host,
                      unsigned int port,
                      unsigned int initial_room);

void oxl_client_stop(oxl_client_t *self);

void oxl_client_say(oxl_client_t *self,
                    size_t size,
                    uv_buf_t buf,
                    uv_write_cb oxl_client_do_say_cb);

/* Room Section */
typedef struct oxl_net_room_s {
    OXL_NET_HEADER_FIELDS
    int room_flags;
    int face;
    short room_id;
    short room_name_offset;
    short image_name_offset;
    short artist_name_offset;
    short password_offset;
    short hot_spot_count;
    short hot_spot_offset;
    short image_count;
    short image_offset;
    short draw_commands_count;
    short first_draw_command_offset;
    short people_count;
    short loose_prop_count;
    short first_loose_prop_offset;
    short reserved;
    short data_len;
    unsigned char *data;
} oxl_net_room_t;

/* nav messages */
typedef enum oxl_nav_err_e {
    NAV_ERR_INTERNAL_ERROR,
    NAV_ERR_ROOM_UNKNOWN,
    NAV_ERR_ROOM_FULL,
    NAV_ERR_ROOM_CLOSED,
    NAV_ERR_CANT_AUTHOR,
    NAV_ERR_PALACE_FULL
} oxl_nav_err_e;

typedef struct oxl_nav_err_s {
    int ref_id;
    short code;
} oxl_nav_err_t;

void oxl_nav_err_init(oxl_nav_err_t *nav_err,
                      const unsigned char *data,
                      const int ref_id);

void oxl_net_room_init(oxl_net_room_t *self,
                       const unsigned char *data,
                       const int ref_id);

unsigned int oxl_net_room_len(const unsigned char *data);
short oxl_net_room_data_len(const unsigned char *data);
short oxl_net_room_room_name_len(const unsigned char *data);
short oxl_net_room_image_name_len(const unsigned char *data);
void oxl_net_room_room_name(char *room_name,
                            const oxl_net_room_t *room_msg);

void oxl_net_room_image_name(char *image_name,
                             const oxl_net_room_t *room_msg);

void oxl_room_init(oxl_room_t *room);
void oxl_room_selected_user_changed_cb(oxl_user_t *user);

typedef struct oxl_net_goto_room_s {
    OXL_NET_HEADER_FIELDS
    short room_id;
} oxl_net_goto_room_t;

void oxl_net_goto_room_init(oxl_net_goto_room_t *self,
                            unsigned int user_id,
                            short goto_room_id);

void oxl_client_leave_room_sync(oxl_client_t *self);
void oxl_client_join_room(oxl_client_t *self, uint goto_room_id, void *cb);
/* void oxl_client_goto_room(oxl_client_t *self, unsigned int goto_room_id); */

void oxl_dump_buf(const uv_buf_t buf, size_t size);
    
/* don't export */
#undef OXL_NET_HEADER_FIELDS

#ifdef __cplusplus
}
#endif
#endif /* OXYALE_H_ */
