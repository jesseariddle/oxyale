#include "env.h"
#include "opal_chat_message.h"

opalChatMessage_t *
OPAL_MakeChatMessage( byte *data );
{
    opalChatMessage_t cm = malloc( sizeof (*cm) );

    

    return cm;
}

void
OPAL_FreeChatMessage( opalChatMessage_t *cm )
{
    free( cm );
}
