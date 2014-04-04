#pragma once
#ifndef OPAL_MESSAGE_H_
#define OPAL_MESSAGE_H_

/* public interface */

#define OPAL_ROOM_HEADER_LEN 48
#define OPAL_ROOM_PREFIX_LEN 8

typedef struct {
    int32_t referenceId;
    int8_t errorCode;
} opalNavErrorDescriptor_t;

extern opalNavErrorDescriptor_t *OPAL_MakeNavErrorDescriptor( byte *data, int32_t refId );
extern void OPAL_FreeNavErrorDescriptor( opalNavErrorDescriptor_t *ned );

typedef struct {
    int32_t messageId;
    int32_t referenceId;

    int32_t roomFlags;
    int32_t face;
    int16_t roomId;
    int16_t roomNameOffset;
    int16_t imageNameOffset;
    int16_t artistNameOffset;
    int16_t passwordOffset;
    int16_t hotSpotCount;
    int16_t hotSpotOffset;
    int16_t imageCount;
    int16_t imageOffset;
    int16_t drawCommandsCount;
    int16_t firstDrawCommandOffset;
    int16_t peopleCount;
    int16_t loosePropCount;
    int16_t firstLoosePropOffset;
    int16_t reserved;
    int16_t roomDataLength;
    int8_t  *roomBytes;
    char *roomName;
    char *backgroundImageName;
} opalRoomDescriptor_t;

extern opalRoomDescriptor_t *OPAL_MakeRoomDescriptor( byte *data, int32_t refId );
extern void OPAL_FreeRoomDescriptor( );

/* nav messages */

enum {
    INTERNAL_ERROR,
    ROOM_UNKNOWN,
    ROOM_FULL,
    ROOM_CLOSED,
    CANT_AUTHOR,
    PALACE_FULL
} opalNavMsg_e;

/* can't seem to decide on enum or defines */

#define          OPAL_MSG_NAV_INTERNAL_ERROR 0x00000000
#define            OPAL_MSG_NAV_ROOM_UNKNOWN 0x00000001
#define               OPAL_MSG_NAV_ROOM_FULL 0x00000002
#define             OPAL_MSG_NAV_ROOM_CLOSED 0x00000003
#define             OPAL_MSG_NAV_CANT_AUTHOR 0x00000004
#define             OPAL_MSG_NAV_PALACE_FULL 0x00000005

/* incoming messages */

#define           OPAL_MSG_IN_UNKNOWN_SERVER 0x70736572
#define     OPAL_MSG_IN_LITTLE_ENDIAN_SERVER 0x72796974
#define        OPAL_MSG_IN_BIG_ENDIAN_SERVER 0x74697972

#define    OPAL_MSG_IN_ALTERNATE_LOGON_REPLY 0x72657032

#define           OPAL_MSG_IN_SERVER_VERSION 0x76657273
#define              OPAL_MSG_IN_SERVER_INFO 0x73696E66
#define              OPAL_MSG_IN_USER_STATUS 0x75537461
#define   OPAL_MSG_IN_USER_LOGGED_ON_AND_MAX 0x6C6F6720
#define OPAL_MSG_IN_GOT_HTTP_SERVER_LOCATION 0x48545450
#define     OPAL_MSG_IN_GOT_ROOM_DESCRIPTION 0x726F6F6D
#define OPAL_MSG_IN_GOT_ROOM_DESCRIPTION_ALT 0x73526F6D
#define            OPAL_MSG_IN_GOT_USER_LIST 0x72707273
#define            OPAL_MSG_IN_GOT_ROOM_LIST 0x724C7374
#define             OPAL_MSG_IN_ROOM_DESCEND 0x656E6472
#define                 OPAL_MSG_IN_USER_NEW 0x6E707273

#define                     OPAL_MSG_IN_PING 0x70696E67

#define                    OPAL_MSG_IN_XTALK 0x78746C6B
#define                 OPAL_MSG_IN_XWHISPER 0x78776973
#define                     OPAL_MSG_IN_TALK 0x74616C6B
#define                  OPAL_MSG_IN_WHISPER 0x77686973
#define                 OPAL_MSG_IN_MOVEMENT 0x754C6F63
#define               OPAL_MSG_IN_USER_COLOR 0x75737243
#define         OPAL_MSG_IN_USER_DESCRIPTION 0x75737244
#define                OPAL_MSG_IN_USER_FACE 0x75737246
#define                OPAL_MSG_IN_USER_PROP 0x75737250
#define              OPAL_MSG_IN_USER_RENAME 0x7573724E
#define             OPAL_MSG_IN_USER_LEAVING 0x62796520
#define          OPAL_MSG_IN_CONNECTION_DIED 0x646F776E
#define            OPAL_MSG_IN_INCOMING_FILE 0x7346696C
#define           OPAL_MSG_IN_ASSET_INCOMING 0x73417374
#define           OPAL_MSG_IN_USER_EXIT_ROOM 0x65707273
#define            OPAL_MSG_IN_GOT_ALL_ROOMS 0x724C7374
#define   OPAL_MSG_IN_GOT_REPLY_OF_ALL_USERS 0x754C7374

#define                OPAL_MSG_IN_DOOR_LOCK 0x6C6F636B
#define              OPAL_MSG_IN_DOOR_UNLOCK 0x756E6C6F
#define               OPAL_MSG_IN_SPOT_STATE 0x73537461
#define                OPAL_MSG_IN_SPOT_MOVE 0x636F4C73
#define                OPAL_MSG_IN_PICT_MOVE 0x704C6F63
#define                 OPAL_MSG_IN_DRAW_CMD 0x64726177

#define                OPAL_MSG_IN_PROP_MOVE 0x6D507270
#define              OPAL_MSG_IN_PROP_DELETE 0x64507270
#define                 OPAL_MSG_IN_PROP_NEW 0x6E507270

#define              OPAL_MSG_IN_ASSET_QUERY 0x71417374
#define                OPAL_MSG_IN_NAV_ERROR 0x73457272
#define              OPAL_MSG_IN_SERVER_DOWN 0x646F776E

#define                 OPAL_MSG_IN_BLOWTHRU 0x626C6F77

#define             OPAL_MSG_IN_AUTHENTICATE 0x61757468

/* outgoing messages */

#define                     OPAL_MSG_OUT_BYE 0x62796520
#define                    OPAL_MSG_OUT_PONG 0x706F6E67
#define                     OPAL_MSG_OUT_SAY 0x78746C6B
#define                 OPAL_MSG_OUT_WHISPER 0x78776973
#define                    OPAL_MSG_OUT_MOVE 0x754C6F63
#define              OPAL_MSG_OUT_USER_COLOR 0x75737243
#define               OPAL_MSG_OUT_USER_FACE 0x75737246
#define       OPAL_MSG_OUT_REQUEST_ROOM_LIST 0x724C7374
#define               OPAL_MSG_OUT_GOTO_ROOM 0x6E617652
#define       OPAL_MSG_OUT_REQUEST_USER_LIST 0x754C7374
#define           OPAL_MSG_OUT_REQUEST_ASSET 0x71417374
#define               OPAL_MSG_OUT_USER_PROP 0x75737250
#define             OPAL_MSG_OUT_CHANGE_NAME 0x7573724E
#define                OPAL_MSG_OUT_BLOWTHRU 0x626C6f77
#define              OPAL_MSG_OUT_SPOT_STATE 0x73537461
#define               OPAL_MSG_OUT_DOOR_LOCK 0x6C6F636B
#define             OPAL_MSG_OUT_DOOR_UNLOCK 0x756E6C6F
#define               OPAL_MSG_OUT_SUPERUSER 0x73757372
#define                   OPAL_MSG_OUT_LOGON 0x72656769
#define               OPAL_MSG_OUT_PROP_MOVE 0x6D507270
#define             OPAL_MSG_OUT_PROP_DELETE 0x64507270
#define                OPAL_MSG_OUT_PROP_NEW 0x6E507270
#define              OPAL_MSG_OUT_ASSET_REGI 0x72417374
#define              OPAL_MSG_OUT_GLOBAL_MSG 0x676D7367
#define                OPAL_MSG_OUT_ROOM_MSG 0x726D7367
#define                OPAL_MSG_OUT_SUSR_MSG 0x736D7367
#define            OPAL_MSG_OUT_AUTHRESPONSE 0x61757472
#define                    OPAL_MSG_OUT_DRAW 0x64726177

#endif /* OPAL_MESSAGE_H_ */
