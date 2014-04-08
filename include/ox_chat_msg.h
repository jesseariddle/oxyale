#pragma once
#ifndef OX_CHAT_MSG_H_
#define OX_CHAT_MSG_H_

typedef struct {
    
} ox_chat_bubble_t;

typedef struct {
    char *text;
    int8_t is_whisper;
    int32_t x;
    int32_t y;
    uint32_t tint;
    /* ox_user_t *user; */
    ox_chat_bubble_t *chat_bubble;
    uint32_t display_failure_count;
    uint8_t is_sticky_bubble;
} ox_chat_msg_t;

/* void ox_chat_msg_init( ox_chat_msg_t *chat_msg, const byte *data ); */

#endif /* OX_CHAT_MSG_H_ */
