#include "env.h"
#include "opal_string.h"
#include "opal_message.h"

opalNavErrorDescriptor_t *
OPAL_MakeNavErrorDescriptor( byte *data, int32_t refId )
{
    opalNavErrorDescriptor_t *ned = malloc( sizeof (*ned) );
    ned->referenceId = refId;
    ned->errorCode = *data;

    return ned;
}

void
OPAL_FreeNavErrorDescriptor( opalNavErrorDescriptor_t *ned )
{
    free( ned );
}

opalRoomDescriptor_t *
OPAL_MakeRoomDescriptor( byte *data, int32_t refId )
{
    opalRoomDescriptor_t *rd;

    /* allocate memory for descriptor */
    rd = malloc( sizeof (*rd) );

    /* copy prefix info */
    rd->messageId = OPAL_MSG_IN_GOT_ROOM_DESCRIPTION;
    rd->referenceId = refId;

    /* copy room header */
    memcpy( rd, data + OPAL_ROOM_PREFIX_LEN, OPAL_ROOM_HEADER_LEN );

    /* allocate memory for room data */
    rd->roomBytes = calloc( rd->roomDataLength, sizeof (*rd->roomBytes) );

    /* copy room data */
    memcpy( rd->roomBytes, data + OPAL_ROOM_HEADER_LEN, rd->roomDataLength );

    /* allocate memory for room name */
    uint8_t roomNameLen = rd->roomBytes[rd->roomNameOffset];
    rd->roomName = calloc( roomNameLen + 1, sizeof (*rd->roomName) );

    /* copy room name */
    memcpy( rd->roomName, data + OPAL_ROOM_HEADER_LEN + rd->roomNameOffset + sizeof (roomNameLen), roomNameLen );

    /* allocate memory for background image name */
    uint8_t bgImageNameLen = rd->roomBytes[rd->imageNameOffset];
    rd->backgroundImageName = calloc( bgImageNameLen + 1, sizeof (*rd->backgroundImageName ) );

    /* copy background image name */
    memcpy( rd->backgroundImageName, data + OPAL_ROOM_HEADER_LEN + rd->imageNameOffset + sizeof (bgImageNameLen), bgImageNameLen );

    return rd;
}

void 
OPAL_FreeRoomDescriptor( opalRoomDescriptor_t *rd )
{
    free( rd->backgroundImageName );
    free( rd->roomName );
    free( rd->roomBytes );
    free( rd );
}
