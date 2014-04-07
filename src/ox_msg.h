#pragma once
#ifndef OX_MSG_H_
#define OX_MSG_H_

/* public interface */

int32_t OX_ROOM_PREFIX_LEN = 4;
int32_t OX_ROOM_HEADER_LEN = 48;

typedef struct {
    uint8_t length;
    char *text;
} ox_uint8_sized_string_t;

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

const int32_t          OX_MSG_NAV_INTERNAL_ERROR = 0x00000000;
const int32_t            OX_MSG_NAV_ROOM_UNKNOWN = 0x00000001;
const int32_t               OX_MSG_NAV_ROOM_FULL = 0x00000002;
const int32_t             OX_MSG_NAV_ROOM_CLOSED = 0x00000003;
const int32_t             OX_MSG_NAV_CANT_AUTHOR = 0x00000004;
const int32_t             OX_MSG_NAV_PALACE_FULL = 0x00000005;

/* incoming messages */

const int32_t           OX_MSG_IN_UNKNOWN_SERVER = 0x70736572;
const int32_t     OX_MSG_IN_LITTLE_ENDIAN_SERVER = 0x72796974;
const int32_t        OX_MSG_IN_BIG_ENDIAN_SERVER = 0x74697972;

const int32_t    OX_MSG_IN_ALTERNATE_LOGON_REPLY = 0x72657032;

const int32_t           OX_MSG_IN_SERVER_VERSION = 0x76657273;
const int32_t              OX_MSG_IN_SERVER_INFO = 0x73696E66;
const int32_t              OX_MSG_IN_USER_STATUS = 0x75537461;
const int32_t   OX_MSG_IN_USER_LOGGED_ON_AND_MAX = 0x6C6F6720;
const int32_t OX_MSG_IN_GOT_HTTP_SERVER_LOCATION = 0x48545450;
const int32_t     OX_MSG_IN_GOT_ROOM_DESCRIPTION = 0x726F6F6D;
const int32_t OX_MSG_IN_GOT_ROOM_DESCRIPTION_ALT = 0x73526F6D;
const int32_t            OX_MSG_IN_GOT_USER_LIST = 0x72707273;
const int32_t            OX_MSG_IN_GOT_ROOM_LIST = 0x724C7374;
const int32_t             OX_MSG_IN_ROOM_DESCEND = 0x656E6472;
const int32_t                 OX_MSG_IN_USER_NEW = 0x6E707273;

const int32_t                     OX_MSG_IN_PING = 0x70696E67;

const int32_t                    OX_MSG_IN_XTALK = 0x78746C6B;
const int32_t                 OX_MSG_IN_XWHISPER = 0x78776973;
const int32_t                     OX_MSG_IN_TALK = 0x74616C6B;
const int32_t                  OX_MSG_IN_WHISPER = 0x77686973;
const int32_t                 OX_MSG_IN_MOVEMENT = 0x754C6F63;
const int32_t               OX_MSG_IN_USER_COLOR = 0x75737243;
const int32_t         OX_MSG_IN_USER_DESCRIPTION = 0x75737244;
const int32_t                OX_MSG_IN_USER_FACE = 0x75737246;
const int32_t                OX_MSG_IN_USER_PROP = 0x75737250;
const int32_t              OX_MSG_IN_USER_RENAME = 0x7573724E;
const int32_t             OX_MSG_IN_USER_LEAVING = 0x62796520;
const int32_t          OX_MSG_IN_CONNECTION_DIED = 0x646F776E;
const int32_t            OX_MSG_IN_INCOMING_FILE = 0x7346696C;
const int32_t           OX_MSG_IN_ASSET_INCOMING = 0x73417374;
const int32_t           OX_MSG_IN_USER_EXIT_ROOM = 0x65707273;
const int32_t            OX_MSG_IN_GOT_ALL_ROOMS = 0x724C7374;
const int32_t   OX_MSG_IN_GOT_REPLY_OF_ALL_USERS = 0x754C7374;

const int32_t                OX_MSG_IN_DOOR_LOCK = 0x6C6F636B;
const int32_t              OX_MSG_IN_DOOR_UNLOCK = 0x756E6C6F;
const int32_t               OX_MSG_IN_SPOT_STATE = 0x73537461;
const int32_t                OX_MSG_IN_SPOT_MOVE = 0x636F4C73;
const int32_t                OX_MSG_IN_PICT_MOVE = 0x704C6F63;
const int32_t                 OX_MSG_IN_DRAW_CMD = 0x64726177;

const int32_t                OX_MSG_IN_PROP_MOVE = 0x6D507270;
const int32_t              OX_MSG_IN_PROP_DELETE = 0x64507270;
const int32_t                 OX_MSG_IN_PROP_NEW = 0x6E507270;

const int32_t              OX_MSG_IN_ASSET_QUERY = 0x71417374;
const int32_t                OX_MSG_IN_NAV_ERROR = 0x73457272;
const int32_t              OX_MSG_IN_SERVER_DOWN = 0x646F776E;

const int32_t                 OX_MSG_IN_BLOWTHRU = 0x626C6F77;

const int32_t             OX_MSG_IN_AUTHENTICATE = 0x61757468;

/* outgoing messages */

const int32_t                     OX_MSG_OUT_BYE = 0x62796520;
const int32_t                    OX_MSG_OUT_PONG = 0x706F6E67;
const int32_t                     OX_MSG_OUT_SAY = 0x78746C6B;
const int32_t                 OX_MSG_OUT_WHISPER = 0x78776973;
const int32_t                    OX_MSG_OUT_MOVE = 0x754C6F63;
const int32_t              OX_MSG_OUT_USER_COLOR = 0x75737243;
const int32_t               OX_MSG_OUT_USER_FACE = 0x75737246;
const int32_t       OX_MSG_OUT_REQUEST_ROOM_LIST = 0x724C7374;
const int32_t               OX_MSG_OUT_GOTO_ROOM = 0x6E617652;
const int32_t       OX_MSG_OUT_REQUEST_USER_LIST = 0x754C7374;
const int32_t           OX_MSG_OUT_REQUEST_ASSET = 0x71417374;
const int32_t               OX_MSG_OUT_USER_PROP = 0x75737250;
const int32_t             OX_MSG_OUT_CHANGE_NAME = 0x7573724E;
const int32_t                OX_MSG_OUT_BLOWTHRU = 0x626C6f77;
const int32_t              OX_MSG_OUT_SPOT_STATE = 0x73537461;
const int32_t               OX_MSG_OUT_DOOR_LOCK = 0x6C6F636B;
const int32_t             OX_MSG_OUT_DOOR_UNLOCK = 0x756E6C6F;
const int32_t               OX_MSG_OUT_SUPERUSER = 0x73757372;
const int32_t                   OX_MSG_OUT_LOGON = 0x72656769;
const int32_t               OX_MSG_OUT_PROP_MOVE = 0x6D507270;
const int32_t             OX_MSG_OUT_PROP_DELETE = 0x64507270;
const int32_t                OX_MSG_OUT_PROP_NEW = 0x6E507270;
const int32_t              OX_MSG_OUT_ASSET_REGI = 0x72417374;
const int32_t              OX_MSG_OUT_GLOBAL_MSG = 0x676D7367;
const int32_t                OX_MSG_OUT_ROOM_MSG = 0x726D7367;
const int32_t                OX_MSG_OUT_SUSR_MSG = 0x736D7367;
const int32_t            OX_MSG_OUT_AUTHRESPONSE = 0x61757472;
const int32_t                    OX_MSG_OUT_DRAW = 0x64726177;

#endif /* OX_MSG_H_ */
