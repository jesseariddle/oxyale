#pragma once
#ifndef OX_NETMSG_H_
#define OX_NETMSG_H_

/* public interface */

#define OX_ROOM_PREFIX_LEN 4
#define OX_ROOM_HEADER_LEN 48

typedef struct {
    int32_t ref_id;
    int8_t code;
} ox_nav_err_t;

void ox_nav_err_init( ox_nav_err_t *nav_err, const byte *data, const int32_t ref_id );

typedef struct {
    int32_t msg_id;
    int32_t ref_id;

    int32_t room_flags;
    int32_t face;
    int16_t room_id;
    int16_t room_name_offset;
    int16_t image_name_offset;
    int16_t artist_name_offset;
    int16_t password_offset;
    int16_t hot_spot_count;
    int16_t hot_spot_offset;
    int16_t image_count;
    int16_t image_offset;
    int16_t draw_commands_count;
    int16_t first_draw_command_offset;
    int16_t people_count;
    int16_t loose_prop_count;
    int16_t first_loose_prop_offset;
    int16_t reserved;
    int16_t data_len;
    byte    *data;
} ox_room_msg_t;

void ox_room_msg_init( ox_room_msg_t *room_msg, const byte *data, const int32_t ref_id );
uint32_t ox_room_msg_len( const byte *data );
uint16_t ox_room_msg_data_len( const byte *data );
uint8_t ox_room_msg_room_name_len( const byte *data );
uint8_t ox_room_msg_image_name_len( const byte *data );
void ox_room_msg_room_name( char *room_name, const ox_room_msg_t *room_msg );
void ox_room_msg_image_name( char *image_name, const ox_room_msg_t *room_msg );

/* nav messages */

enum {
    INTERNAL_ERROR,
    ROOM_UNKNOWN,
    ROOM_FULL,
    ROOM_CLOSED,
    CANT_AUTHOR,
    PALACE_FULL
} ox_nav_msg_e;

/* can't seem to decide on enum or defines */

#define          OX_NETMSG_NAV_INTERNAL_ERROR 0x00000000
#define            OX_NETMSG_NAV_ROOM_UNKNOWN 0x00000001
#define               OX_NETMSG_NAV_ROOM_FULL 0x00000002
#define             OX_NETMSG_NAV_ROOM_CLOSED 0x00000003
#define             OX_NETMSG_NAV_CANT_AUTHOR 0x00000004
#define             OX_NETMSG_NAV_PALACE_FULL 0x00000005

/* incoming messages */

#define           OX_NETMSG_IN_UNKNOWN_SERVER 0x70736572
#define     OX_NETMSG_IN_LITTLE_ENDIAN_SERVER 0x72796974
#define        OX_NETMSG_IN_BIG_ENDIAN_SERVER 0x74697972

#define    OX_NETMSG_IN_ALTERNATE_LOGON_REPLY 0x72657032

#define           OX_NETMSG_IN_SERVER_VERSION 0x76657273
#define              OX_NETMSG_IN_SERVER_INFO 0x73696E66
#define              OX_NETMSG_IN_USER_STATUS 0x75537461
#define   OX_NETMSG_IN_USER_LOGGED_ON_AND_MAX 0x6C6F6720
#define OX_NETMSG_IN_GOT_HTTP_SERVER_LOCATION 0x48545450
#define     OX_NETMSG_IN_GOT_ROOM_DESCRIPTION 0x726F6F6D
#define OX_NETMSG_IN_GOT_ROOM_DESCRIPTION_ALT 0x73526F6D
#define            OX_NETMSG_IN_GOT_USER_LIST 0x72707273
#define            OX_NETMSG_IN_GOT_ROOM_LIST 0x724C7374
#define             OX_NETMSG_IN_ROOM_DESCEND 0x656E6472
#define                 OX_NETMSG_IN_USER_NEW 0x6E707273

#define                     OX_NETMSG_IN_PING 0x70696E67

#define                    OX_NETMSG_IN_XTALK 0x78746C6B
#define                 OX_NETMSG_IN_XWHISPER 0x78776973
#define                     OX_NETMSG_IN_TALK 0x74616C6B
#define                  OX_NETMSG_IN_WHISPER 0x77686973
#define                 OX_NETMSG_IN_MOVEMENT 0x754C6F63
#define               OX_NETMSG_IN_USER_COLOR 0x75737243
#define         OX_NETMSG_IN_USER_DESCRIPTION 0x75737244
#define                OX_NETMSG_IN_USER_FACE 0x75737246
#define                OX_NETMSG_IN_USER_PROP 0x75737250
#define              OX_NETMSG_IN_USER_RENAME 0x7573724E
#define             OX_NETMSG_IN_USER_LEAVING 0x62796520
#define          OX_NETMSG_IN_CONNECTION_DIED 0x646F776E
#define            OX_NETMSG_IN_INCOMING_FILE 0x7346696C
#define           OX_NETMSG_IN_ASSET_INCOMING 0x73417374
#define           OX_NETMSG_IN_USER_EXIT_ROOM 0x65707273
#define            OX_NETMSG_IN_GOT_ALL_ROOMS 0x724C7374
#define   OX_NETMSG_IN_GOT_REPLY_OF_ALL_USERS 0x754C7374

#define                OX_NETMSG_IN_DOOR_LOCK 0x6C6F636B
#define              OX_NETMSG_IN_DOOR_UNLOCK 0x756E6C6F
#define               OX_NETMSG_IN_SPOT_STATE 0x73537461
#define                OX_NETMSG_IN_SPOT_MOVE 0x636F4C73
#define                OX_NETMSG_IN_PICT_MOVE 0x704C6F63
#define                 OX_NETMSG_IN_DRAW_CMD 0x64726177

#define                OX_NETMSG_IN_PROP_MOVE 0x6D507270
#define              OX_NETMSG_IN_PROP_DELETE 0x64507270
#define                 OX_NETMSG_IN_PROP_NEW 0x6E507270

#define              OX_NETMSG_IN_ASSET_QUERY 0x71417374
#define                OX_NETMSG_IN_NAV_ERROR 0x73457272
#define              OX_NETMSG_IN_SERVER_DOWN 0x646F776E

#define                 OX_NETMSG_IN_BLOWTHRU 0x626C6F77

#define             OX_NETMSG_IN_AUTHENTICATE 0x61757468

/* outgoing messages */

#define                     OX_NETMSG_OUT_BYE 0x62796520
#define                    OX_NETMSG_OUT_PONG 0x706F6E67
#define                     OX_NETMSG_OUT_SAY 0x78746C6B
#define                 OX_NETMSG_OUT_WHISPER 0x78776973
#define                    OX_NETMSG_OUT_MOVE 0x754C6F63
#define              OX_NETMSG_OUT_USER_COLOR 0x75737243
#define               OX_NETMSG_OUT_USER_FACE 0x75737246
#define       OX_NETMSG_OUT_REQUEST_ROOM_LIST 0x724C7374
#define               OX_NETMSG_OUT_GOTO_ROOM 0x6E617652
#define       OX_NETMSG_OUT_REQUEST_USER_LIST 0x754C7374
#define           OX_NETMSG_OUT_REQUEST_ASSET 0x71417374
#define               OX_NETMSG_OUT_USER_PROP 0x75737250
#define             OX_NETMSG_OUT_CHANGE_NAME 0x7573724E
#define                OX_NETMSG_OUT_BLOWTHRU 0x626C6f77
#define              OX_NETMSG_OUT_SPOT_STATE 0x73537461
#define               OX_NETMSG_OUT_DOOR_LOCK 0x6C6F636B
#define             OX_NETMSG_OUT_DOOR_UNLOCK 0x756E6C6F
#define               OX_NETMSG_OUT_SUPERUSER 0x73757372
#define                   OX_NETMSG_OUT_LOGON 0x72656769
#define               OX_NETMSG_OUT_PROP_MOVE 0x6D507270
#define             OX_NETMSG_OUT_PROP_DELETE 0x64507270
#define                OX_NETMSG_OUT_PROP_NEW 0x6E507270
#define              OX_NETMSG_OUT_ASSET_REGI 0x72417374
#define              OX_NETMSG_OUT_GLOBAL_MSG 0x676D7367
#define                OX_NETMSG_OUT_ROOM_MSG 0x726D7367
#define                OX_NETMSG_OUT_SUSR_MSG 0x736D7367
#define            OX_NETMSG_OUT_AUTHRESPONSE 0x61757472
#define                    OX_NETMSG_OUT_DRAW 0x64726177

#endif /* OX_NETMSG_H_ */
