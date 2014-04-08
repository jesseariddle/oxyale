#include "../include/env.h"
#include "../include/ox_str.h"
#include "../include/ox_msg.h"

void ox_nav_err_init( ox_nav_err_t *nav_err, const byte *data, const int32_t ref_id )
{
    nav_err->ref_id = ref_id;
    nav_err->code = *data;
}

uint16_t ox_room_msg_data_len( const byte *data )
{
    const ox_room_msg_t *room_msg = (const ox_room_msg_t *)data;
    return room_msg->data_len;
}

uint8_t ox_room_msg_room_name_len( const byte *data )
{
    const ox_room_msg_t *room_msg = (const ox_room_msg_t *)data;
    return room_msg->data[room_msg->room_name_offset];
}

uint8_t ox_room_msg_image_name_len( const byte *data )
{
    const ox_room_msg_t *room_msg = (const ox_room_msg_t *)data;
    return room_msg->data[room_msg->image_name_offset];
}

uint32_t ox_room_msg_len( const byte *data )
{
    const ox_room_msg_t *room_msg = (const ox_room_msg_t *)data;
    return sizeof (*room_msg) + room_msg->data_len;
}

void ox_room_msg_room_name( char *room_name, const ox_room_msg_t *room_msg )
{
    memcpy( room_name, room_msg->data, room_msg->data[room_msg->room_name_offset + 1] );
    room_name[room_msg->data[room_msg->room_name_offset]] = '\0';
}

void ox_room_msg_image_name( char *image_name, const ox_room_msg_t *room_msg )
{
    memcpy( image_name, room_msg->data, room_msg->data[room_msg->image_name_offset + 1] );
    image_name[room_msg->data[room_msg->image_name_offset]] = '\0';
}

void ox_room_msg_init( ox_room_msg_t *room_msg, const byte *data, const int32_t ref_id )
{
    room_msg->msg_id = OX_MSG_IN_GOT_ROOM_DESCRIPTION;
    room_msg->ref_id = ref_id;
    ox_room_msg_t *rd = (ox_room_msg_t *)data;
    memcpy( room_msg, data + OX_ROOM_PREFIX_LEN, OX_ROOM_HEADER_LEN + rd->data_len );
}
