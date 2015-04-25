#include <oxyale.h>

void oxl_net_logon_init(oxl_net_logon_t *self, 
                        char *username,
                        char *wizpass,
                        int initial_room_id,
                        unsigned int reg_crc,
                        int reg_counter,
                        unsigned int puid_crc,
                        int puid_counter)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)buf->base; */
    fprintf(stderr, "--- DEBUG: oxl_net_logon_init\n");
    self->msg_id = OXL_NET_TX_LOGON;
    self->msg_len = 0x80;
    self->msg_ref = 0;
    self->reg_crc = reg_crc;
    self->reg_counter = reg_counter;
    self->username_len = (unsigned char)strlen(username);
    strncpy(self->username, username, OXL_CLIENT_USERNAME_SZ_CAP);
    strncpy(self->wizpass, wizpass, OXL_CLIENT_WIZ_PASS_SZ_CAP);

    /* raw */
    self->flags = OXL_NET_AUXFLAGS_AUTHENTICATE | OXL_NET_AUXFLAGS_WIN32;
    
    self->puid_counter = puid_counter;
    self->puid_crc = puid_crc;
    self->demo_elapsed = 0;
    self->total_elapsed = 0;
    self->demo_limit = 0;

     /* initial room */
    self->initial_room_id = initial_room_id;

     /* does nothing, but is logged by server */
    /* strncpy(self->reserved, "OXYALE", 6); */
    /* strncpy(self->reserved, "PC4232", 6); */
    strncpy(self->reserved, "OPNPAL", 6);

     /* ignored on server */
    self->ul_upload_requested_protocol_version = 0;

     /* TODO upload capabilities plox */
    self->ul_upload_capabilities = OXL_NET_ULCAPS_ASSETS_PALACE;

     /* TODO download capabilities plox */
    self->ul_download_capabilities =
        OXL_NET_DLCAPS_ASSETS_PALACE |
        OXL_NET_DLCAPS_FILES_PALACE |
        OXL_NET_DLCAPS_FILES_HTTPSRVR;

     /* unused */
    self->ul_2d_engine_capabilities = 0;

     /* unused */
    self->ul_2d_graphics_capabilities = 0;

     /* unused */
    self->ul_3d_engine_capabilities = 0;
}

void oxl_net_logon_dump(const oxl_net_logon_t *self)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)logon_buf->base; */
    /* raw */
    fprintf(stderr, "logon->message_id = 0x%x\n", self->msg_id);
    fprintf(stderr, "logon->message_size = %d\n", self->msg_len);
    fprintf(stderr, "logon->message_ref = 0x%x\n", self->msg_ref);
    fprintf(stderr, "logon->reg_crc = 0x%x\n", self->reg_crc);
    fprintf(stderr, "logon->reg_counter = 0x%x\n", self->reg_counter);
    fprintf(stderr, "logon->username_len = %d\n", self->username_len);
    fprintf(stderr, "logon->username = %s\n", self->username);
    fprintf(stderr, "logon->flags = 0x%x\n", self->flags);
    fprintf(stderr, "logon->puid_counter = 0x%x\n", self->puid_counter);
    fprintf(stderr, "logon->puid_crc = 0x%x\n", self->puid_crc);
    fprintf(stderr, "logon->demo_elapsed = 0x%x\n", self->demo_elapsed);
    fprintf(stderr, "logon->total_elapsed = 0x%x\n", self->total_elapsed);
    fprintf(stderr, "logon->demo_limit = 0x%x\n", self->demo_limit);

    /* initial room */
    fprintf(stderr, "logon->room_id = %d\n", self->initial_room_id); 

    /* does nothing, but is logged by server */
    fprintf(stderr, "logon->reserved = %s\n", self->reserved); 
    
    /* ignored on server */
    fprintf(stderr,
            "logon->ul_upload_requested_protocol_version = 0x%x\n",
            self->ul_upload_requested_protocol_version); 

    /* TODO upload capabilities plox */
    fprintf(stderr,
            "logon->ul_upload_capabilities = 0x%x\n",
            self->ul_upload_capabilities); 

    /* TODO download capabilities plox */
    fprintf(stderr,
            "logon->ul_download_capabilities = 0x%x\n",
            self->ul_download_capabilities);
    
    /* unused */   
    fprintf(stderr,
            "logon->ul_2d_engine_capabilities = 0x%x\n",
            self->ul_2d_engine_capabilities);

    /* unused */
    fprintf(stderr,
            "logon->ul_2d_graphics_capabilities = 0x%x\n",
            self->ul_2d_graphics_capabilities);

    /* unused */
    fprintf(stderr,
            "logon->ul_3d_engine_capabilities = 0x%x\n",
            self->ul_3d_engine_capabilities);
}

void oxl_net_goto_room_init(oxl_net_goto_room_t *self,
                            unsigned int user_id,
                            short room_id)
{
    self->msg_id = OXL_NET_TX_GOTO_ROOM;
    self->msg_len = sizeof(short);
    self->msg_ref = user_id;
    self->room_id = room_id;
}

void oxl_net_say_dump(oxl_net_say_t *self)
{
    fprintf(stderr, "say->msg_id = 0x%x\n", self->msg_id);
    fprintf(stderr, "say->msg_size = %d\n", self->msg_len);
    fprintf(stderr, "say->msg_ref = 0x%x\n", self->msg_ref);
    fprintf(stderr, "say->len = %d\n", self->len);
    fprintf(stderr, "say->msg = %s\n", self->msg);
}

void oxl_net_say_init(oxl_net_say_t *self, oxl_client_t *client, char *text)
{
    /* oxl_say_t *say_msg = (oxl_say_t *)&buf.base; */
    char ct[OXL_CRYPT_CIPHERTEXT_SZ_CAP];
    oxl_crypt_encrypt(&client->crypt, text, ct);
    fprintf(stderr, "text: \"%s\", ct: \"%s\"\n", text, ct);
    uint z = strnlen(ct, OXL_CRYPT_CIPHERTEXT_SZ_CAP);
    self->msg_id = OXL_NET_TX_SAY;
    self->msg_len = z + 3;
    self->msg_ref = client->user.id;
    self->len = (ushort)z;
    memcpy(self->msg, ct, z);
}

