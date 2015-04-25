#include <oxyale.h>

void oxl_room_init(oxl_room_t *self) {
    self->name = "Not Connected";
    self->dim_level = 1;
    self->should_show_avatars = 1;
    self->user_id = -1;
    self->chat_log = "";
}

void oxl_room_on_selected_user_changed(oxl_user_t *user) {
    
}

