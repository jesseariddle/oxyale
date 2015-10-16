//
//  palcom.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palcom_h
#define oxyale_palcom_h

#include "oxlcom.h"

#define             PAL_INET_DEFAULT_PORT 9998
#define                PAL_NET_HEADER_LEN 12

#define               PAL_USERNAME_SZ_CAP 31
#define                PAL_WIZPASS_SZ_CAP 32
#define            PAL_SERVER_NAME_SZ_CAP 256
#define            PAL_RESERVED_LOGON_LEN 5

#define                   PAL_FQDN_SZ_CAP 256
#define                   PAL_PORT_SZ_CAP 6

#define               PAL_ROOM_PREFIX_LEN 4
#define               PAL_ROOM_HEADER_LEN 48
#define              PAL_ROOM_NAME_SZ_CAP 256

#define                PAL_CRYPTO_LUT_LEN 512
#define                PAL_CRYPTO_LUT_MAX 256
#define       PAL_CRYPTO_PLAINTEXT_SZ_CAP 255
#define      PAL_CRYPTO_CIPHERTEXT_SZ_CAP 255

#define               PAL_AUXFLAGS_OSMASK 0x0000000F
#define         PAL_AUXFLAGS_AUTHENTICATE 0x80000000

#define      PAL_AUXFLAGS_UNKNOWN_MACHINE 0
#define               PAL_AUXFLAGS_MAC68K 1
#define               PAL_AUXFLAGS_MACPPC 2
#define                PAL_AUXFLAGS_WIN16 3
#define                PAL_AUXFLAGS_WIN32 4
#define                 PAL_AUXFLAGS_JAVA 5

#define          PAL_ULCAPS_ASSETS_PALACE 0x00000001
#define             PAL_ULCAPS_ASSETS_FTP 0x00000002
#define            PAL_ULCAPS_ASSETS_HTTP 0x00000004
#define           PAL_ULCAPS_ASSETS_OTHER 0x00000008
#define           PAL_ULCAPS_FILES_PALACE 0x00000010
#define              PAL_ULCAPS_FILES_FTP 0x00000020
#define             PAL_ULCAPS_FILES_HTTP 0x00000040
#define            PAL_ULCAPS_FILES_OTHER 0x00000080
#define             PAL_ULCAPS_EXTEND_PKT 0x00000100

#define          PAL_DLCAPS_ASSETS_PALACE 0x00000001
#define             PAL_DLCAPS_ASSETS_FTP 0x00000002
#define            PAL_DLCAPS_ASSETS_HTTP 0x00000004
#define           PAL_DLCAPS_ASSETS_OTHER 0x00000008
#define           PAL_DLCAPS_FILES_PALACE 0x00000010
#define              PAL_DLCAPS_FILES_FTP 0x00000020
#define             PAL_DLCAPS_FILES_HTTP 0x00000040
#define            PAL_DLCAPS_FILES_OTHER 0x00000080
#define         PAL_DLCAPS_FILES_HTTPSRVR 0x00000100
#define             PAL_DLCAPS_EXTEND_PKT 0x00000200

/* navigation */

#define            PAL_NAV_INTERNAL_ERROR 0x00000000
#define              PAL_NAV_ROOM_UNKNOWN 0x00000001
#define                 PAL_NAV_ROOM_FULL 0x00000002
#define               PAL_NAV_ROOM_CLOSED 0x00000003
#define               PAL_NAV_CANT_AUTHOR 0x00000004
#define               PAL_NAV_PALACE_FULL 0x00000005

/* incoming messages */

#define             PAL_RX_UNKNOWN_SERVER 0x70736572
#define       PAL_RX_LITTLE_ENDIAN_SERVER 0x72796974
#define          PAL_RX_BIG_ENDIAN_SERVER 0x74697972

#define      PAL_RX_ALTERNATE_LOGON_REPLY 0x72657032

#define             PAL_RX_SERVER_VERSION 0x76657273
#define                PAL_RX_SERVER_INFO 0x73696E66
#define                PAL_RX_USER_STATUS 0x75537461
#define     PAL_RX_USER_LOGGED_ON_AND_MAX 0x6C6F6720
#define   PAL_RX_GOT_HTTP_SERVER_LOCATION 0x48545450
#define       PAL_RX_GOT_ROOM_DESCRIPTION 0x726F6F6D
#define   PAL_RX_GOT_ROOM_DESCRIPTION_ALT 0x73526F6D
#define              PAL_RX_GOT_USER_LIST 0x72707273
#define              PAL_RX_GOT_ROOM_LIST 0x724C7374
#define               PAL_RX_ROOM_DESCEND 0x656E6472
#define                   PAL_RX_USER_NEW 0x6E707273

#define                       PAL_RX_PING 0x70696E67

#define                      PAL_RX_XTALK 0x78746C6B
#define                   PAL_RX_XWHISPER 0x78776973
#define                       PAL_RX_TALK 0x74616C6B
#define                    PAL_RX_WHISPER 0x77686973
#define                   PAL_RX_MOVEMENT 0x754C6F63
#define                 PAL_RX_USER_COLOR 0x75737243
#define           PAL_RX_USER_DESCRIPTION 0x75737244
#define                  PAL_RX_USER_FACE 0x75737246
#define                  PAL_RX_USER_PROP 0x75737250
#define                PAL_RX_USER_RENAME 0x7573724E
#define               PAL_RX_USER_LEAVING 0x62796520
#define              PAL_RX_INCOMING_FILE 0x7346696C
#define             PAL_RX_ASSET_INCOMING 0x73417374
#define             PAL_RX_USER_EXIT_ROOM 0x65707273
#define              PAL_RX_GOT_ALL_ROOMS 0x724C7374
#define     PAL_RX_GOT_REPLY_OF_ALL_USERS 0x754C7374

#define                  PAL_RX_DOOR_LOCK 0x6C6F636B
#define                PAL_RX_DOOR_UNLOCK 0x756E6C6F
#define                 PAL_RX_SPOT_STATE 0x73537461
#define                  PAL_RX_SPOT_MOVE 0x636F4C73
#define                  PAL_RX_PICT_MOVE 0x704C6F63
#define                   PAL_RX_DRAW_MSG 0x64726177

#define                  PAL_RX_PROP_MOVE 0x6D507270
#define                PAL_RX_PROP_DELETE 0x64507270
#define                   PAL_RX_PROP_NEW 0x6E507270

#define                PAL_RX_ASSET_QUERY 0x71417374
#define                  PAL_RX_NAV_ERROR 0x73457272
#define           PAL_RX_CONNECTION_ERROR 0x646F776E

#define                   PAL_RX_BLOWTHRU 0x626C6F77

#define               PAL_RX_AUTHENTICATE 0x61757468

/* outgoing messages */

#define                     PAL_TX_BYE_MSG 0x62796520
#define                    PAL_TX_PING_MSG 0x706F6E67
#define                     PAL_TX_SAY_MSG 0x78746C6B
#define                 PAL_TX_WHISPER_MSG 0x78776973
#define                    PAL_TX_MOVE_MSG 0x754C6F63
#define              PAL_TX_USER_COLOR_MSG 0x75737243
#define               PAL_TX_USER_FACE_MSG 0x75737246
#define       PAL_TX_REQUEST_ROOM_LIST_MSG 0x724C7374
#define               PAL_TX_GOTO_ROOM_MSG 0x6E617652
#define       PAL_TX_REQUEST_USER_LIST_MSG 0x754C7374
#define           PAL_TX_REQUEST_ASSET_MSG 0x71417374
#define               PAL_TX_USER_PROP_MSG 0x75737250
#define             PAL_TX_CHANGE_NAME_MSG 0x7573724E
#define                PAL_TX_BLOWTHRU_MSG 0x626C6f77
#define              PAL_TX_SPOT_STATE_MSG 0x73537461
#define               PAL_TX_DOOR_LOCK_MSG 0x6C6F636B
#define             PAL_TX_DOOR_UNLOCK_MSG 0x756E6C6F
#define                    PAL_TX_ROOM_MSG 0x30C6015D
#define               PAL_TX_SUPERUSER_MSG 0x73757372
#define                   PAL_TX_LOGON_MSG 0x72656769
#define               PAL_TX_PROP_MOVE_MSG 0x6D507270
#define             PAL_TX_PROP_DELETE_MSG 0x64507270
#define                PAL_TX_PROP_NEW_MSG 0x6E507270
#define              PAL_TX_ASSET_REGI_MSG 0x72417374
#define              PAL_TX_GLOBAL_MSG_MSG 0x676D7367
#define                PAL_TX_ROOM_MSG_MSG 0x726D7367
#define                PAL_TX_SUSR_MSG_MSG 0x736D7367
#define            PAL_TX_AUTHRESPONSE_MSG 0x61757472
#define                    PAL_TX_DRAW_MSG 0x64726177

#define PAL_HEADER_FIELDS \
/* public */ \
uint32_t id; \
uint32_t len; \
uint32_t ref;

typedef struct OXLPalMsgStruct {
    PAL_HEADER_FIELDS
} OXLPalMsg;

void OXLInitPalMsg(OXLPalMsg *palMsg);
void OXLDumpPalMsg(const OXLPalMsg *palMsg);

#undef PAL_HEADER_FIELDS /* don't export */

#endif
