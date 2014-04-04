#pragma once
#ifndef OPAL_CHAT_MESSAGE_H_
#define OPAL_CHAT_MESSAGE_H_

typedef struct {
    char *text;
    int8_t isWhisper;
    int32_t x;
    int32_t y;
    uint32_t tint;
    opalUser_t *user;
    opalChatBubble_t *chatBubble;
    uint32_t displayFailureCount;
    uint8_t isStickyBubble;
} opalChatMessage_t;

opalChatMessage_t *OPAL_MakeChatMessage( byte *data );
void OPAL_FreeChatMessage( opalChatMessage_t *chatMessage );

#endif /* OPAL_CHAT_MESSAGE_H_ */
