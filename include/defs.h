//
//  defs.h
//  oxyale
//
//  Created by Jesse Riddle on 4/17/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_defs_h
#define oxyale_defs_h

/* Client Section */
#define              OXL_INET_PORT_SZ_CAP 6
#define              OXL_INET_ADDR_SZ_CAP 16
#define         OXL_INET6_MAX_ADDR_SZ_CAP 46
#define             OXL_INET_DEFAULT_PORT 9998
#define                OXL_NET_HEADER_LEN 12

#define        OXL_CLIENT_USERNAME_SZ_CAP 31
#define        OXL_CLIENT_WIZ_PASS_SZ_CAP 32
#define     OXL_CLIENT_SERVER_NAME_SZ_CAP 256
#define            OXL_CLIENT_FQDN_SZ_CAP 256
#define            OXL_CLIENT_PORT_SZ_CAP 6
#define     OXL_CLIENT_RESERVED_LOGON_LEN 5

#define               OXL_ROOM_PREFIX_LEN 4
#define               OXL_ROOM_HEADER_LEN 48

/* Crypto Section */
#define                 OXL_CRYPT_LUT_LEN 512
#define                 OXL_CRYPT_LUT_MAX 256
#define        OXL_CRYPT_PLAINTEXT_SZ_CAP 255
#define       OXL_CRYPT_CIPHERTEXT_SZ_CAP 255

#define             OXL_NET_AUXFLAGS_OSMASK 0x0000000F
#define       OXL_NET_AUXFLAGS_AUTHENTICATE 0x80000000

#define    OXL_NET_AUXFLAGS_UNKNOWN_MACHINE 0
#define             OXL_NET_AUXFLAGS_MAC68K 1
#define             OXL_NET_AUXFLAGS_MACPPC 2
#define              OXL_NET_AUXFLAGS_WIN16 3
#define              OXL_NET_AUXFLAGS_WIN32 4
#define               OXL_NET_AUXFLAGS_JAVA 5

#define        OXL_NET_ULCAPS_ASSETS_PALACE 0x00000001
#define           OXL_NET_ULCAPS_ASSETS_FTP 0x00000002
#define          OXL_NET_ULCAPS_ASSETS_HTTP 0x00000004
#define         OXL_NET_ULCAPS_ASSETS_OTHER 0x00000008
#define         OXL_NET_ULCAPS_FILES_PALACE 0x00000010
#define            OXL_NET_ULCAPS_FILES_FTP 0x00000020
#define           OXL_NET_ULCAPS_FILES_HTTP 0x00000040
#define          OXL_NET_ULCAPS_FILES_OTHER 0x00000080
#define           OXL_NET_ULCAPS_EXTEND_PKT 0x00000100

#define        OXL_NET_DLCAPS_ASSETS_PALACE 0x00000001
#define           OXL_NET_DLCAPS_ASSETS_FTP 0x00000002
#define          OXL_NET_DLCAPS_ASSETS_HTTP 0x00000004
#define         OXL_NET_DLCAPS_ASSETS_OTHER 0x00000008
#define         OXL_NET_DLCAPS_FILES_PALACE 0x00000010
#define            OXL_NET_DLCAPS_FILES_FTP 0x00000020
#define           OXL_NET_DLCAPS_FILES_HTTP 0x00000040
#define          OXL_NET_DLCAPS_FILES_OTHER 0x00000080
#define       OXL_NET_DLCAPS_FILES_HTTPSRVR 0x00000100
#define           OXL_NET_DLCAPS_EXTEND_PKT 0x00000200

/* navigation */

#define          OXL_NET_NAV_INTERNAL_ERROR 0x00000000
#define            OXL_NET_NAV_ROOM_UNKNOWN 0x00000001
#define               OXL_NET_NAV_ROOM_FULL 0x00000002
#define             OXL_NET_NAV_ROOM_CLOSED 0x00000003
#define             OXL_NET_NAV_CANT_AUTHOR 0x00000004
#define             OXL_NET_NAV_PALACE_FULL 0x00000005

/* incoming messages */

#define           OXL_NET_RX_UNKNOWN_SERVER 0x70736572
#define     OXL_NET_RX_LITTLE_ENDIAN_SERVER 0x72796974
#define        OXL_NET_RX_BIG_ENDIAN_SERVER 0x74697972

#define    OXL_NET_RX_ALTERNATE_LOGON_REPLY 0x72657032

#define           OXL_NET_RX_SERVER_VERSION 0x76657273
#define              OXL_NET_RX_SERVER_INFO 0x73696E66
#define              OXL_NET_RX_USER_STATUS 0x75537461
#define   OXL_NET_RX_USER_LOGGED_ON_AND_MAX 0x6C6F6720
#define OXL_NET_RX_GOT_HTTP_SERVER_LOCATION 0x48545450
#define     OXL_NET_RX_GOT_ROOM_DESCRIPTION 0x726F6F6D
#define OXL_NET_RX_GOT_ROOM_DESCRIPTION_ALT 0x73526F6D
#define            OXL_NET_RX_GOT_USER_LIST 0x72707273
#define            OXL_NET_RX_GOT_ROOM_LIST 0x724C7374
#define             OXL_NET_RX_ROOM_DESCEND 0x656E6472
#define                 OXL_NET_RX_USER_NEW 0x6E707273

#define                     OXL_NET_RX_PING 0x70696E67

#define                    OXL_NET_RX_XTALK 0x78746C6B
#define                 OXL_NET_RX_XWHISPER 0x78776973
#define                     OXL_NET_RX_TALK 0x74616C6B
#define                  OXL_NET_RX_WHISPER 0x77686973
#define                 OXL_NET_RX_MOVEMENT 0x754C6F63
#define               OXL_NET_RX_USER_COLOR 0x75737243
#define         OXL_NET_RX_USER_DESCRIPTION 0x75737244
#define                OXL_NET_RX_USER_FACE 0x75737246
#define                OXL_NET_RX_USER_PROP 0x75737250
#define              OXL_NET_RX_USER_RENAME 0x7573724E
#define             OXL_NET_RX_USER_LEAVING 0x62796520
#define            OXL_NET_RX_INCOMING_FILE 0x7346696C
#define           OXL_NET_RX_ASSET_INCOMING 0x73417374
#define           OXL_NET_RX_USER_EXIT_ROOM 0x65707273
#define            OXL_NET_RX_GOT_ALL_ROOMS 0x724C7374
#define   OXL_NET_RX_GOT_REPLY_OF_ALL_USERS 0x754C7374

#define                OXL_NET_RX_DOOR_LOCK 0x6C6F636B
#define              OXL_NET_RX_DOOR_UNLOCK 0x756E6C6F
#define               OXL_NET_RX_SPOT_STATE 0x73537461
#define                OXL_NET_RX_SPOT_MOVE 0x636F4C73
#define                OXL_NET_RX_PICT_MOVE 0x704C6F63
#define                 OXL_NET_RX_DRAW_CMD 0x64726177

#define                OXL_NET_RX_PROP_MOVE 0x6D507270
#define              OXL_NET_RX_PROP_DELETE 0x64507270
#define                 OXL_NET_RX_PROP_NEW 0x6E507270

#define              OXL_NET_RX_ASSET_QUERY 0x71417374
#define                OXL_NET_RX_NAV_ERROR 0x73457272
#define         OXL_NET_RX_CONNECTION_ERROR 0x646F776E

#define                 OXL_NET_RX_BLOWTHRU 0x626C6F77

#define             OXL_NET_RX_AUTHENTICATE 0x61757468

/* outgoing messages */

#define                     OXL_NET_TX_BYE 0x62796520
#define                    OXL_NET_TX_PONG 0x706F6E67
#define                     OXL_NET_TX_SAY 0x78746C6B
#define                 OXL_NET_TX_WHISPER 0x78776973
#define                    OXL_NET_TX_MOVE 0x754C6F63
#define              OXL_NET_TX_USER_COLOR 0x75737243
#define               OXL_NET_TX_USER_FACE 0x75737246
#define       OXL_NET_TX_REQUEST_ROOM_LIST 0x724C7374
#define               OXL_NET_TX_GOTO_ROOM 0x6E617652
#define       OXL_NET_TX_REQUEST_USER_LIST 0x754C7374
#define           OXL_NET_TX_REQUEST_ASSET 0x71417374
#define               OXL_NET_TX_USER_PROP 0x75737250
#define             OXL_NET_TX_CHANGE_NAME 0x7573724E
#define                OXL_NET_TX_BLOWTHRU 0x626C6f77
#define              OXL_NET_TX_SPOT_STATE 0x73537461
#define               OXL_NET_TX_DOOR_LOCK 0x6C6F636B
#define             OXL_NET_TX_DOOR_UNLOCK 0x756E6C6F
#define                    OXL_NET_TX_ROOM 0x30C6015D
#define               OXL_NET_TX_SUPERUSER 0x73757372
#define                   OXL_NET_TX_LOGON 0x72656769
#define               OXL_NET_TX_PROP_MOVE 0x6D507270
#define             OXL_NET_TX_PROP_DELETE 0x64507270
#define                OXL_NET_TX_PROP_NEW 0x6E507270
#define              OXL_NET_TX_ASSET_REGI 0x72417374
#define              OXL_NET_TX_GLOBAL_MSG 0x676D7367
#define                OXL_NET_TX_ROOM_MSG 0x726D7367
#define                OXL_NET_TX_SUSR_MSG 0x736D7367
#define            OXL_NET_TX_AUTHRESPONSE 0x61757472
#define                    OXL_NET_TX_DRAW 0x64726177

#endif
