//
//  oxyale-common.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_oxyale_common_h
#define oxyale_oxyale_common_h

#include <stdlib.h>

/* typedef unsigned char byte */

/*
 typedef void (*oxl_client_conn_open_start_cb)(void *data);
 typedef void (*oxl_client_conn_open_complete_cb)(void *data);
 typedef void (*oxl_client_conn_failed_cb)(void *data);
 typedef void (*oxl_client_conn_close_cb)(void *data);
 typedef void (*oxl_client_goto_url_cb)(void *data);
 typedef void (*oxl_client_room_changed_cb)(void *data);
 typedef void (*oxl_client_auth_req_cb)(void *data);
 typedef void (*oxl_client_security_err_cb)(void *data);
 */
/*
 typedef void (*oxl_client_logon_cb)(void *data);
 typedef void (*oxl_client_gotoroom_cb)(void *data);
 typedef void (*oxl_client_say_cb)(void *data);
 */

/*
 typedef struct oxl_callbacks_s {
 oxl_client_conn_open_start_cb *client_conn_start_cb;
 oxl_client_conn_open_complete_cb *client_conn_complete_cb;
 oxl_client_conn_failed_cb *client_conn_failed_cb;
 oxl_client_conn_close_cb *client_conn_close_cb;
 oxl_client_goto_url_cb *client_goto_url_cb;
 oxl_client_room_changed_cb *client_room_changed_cb;
 oxl_client_auth_req_cb *client_auth_req_cb;
 oxl_client_security_err_cb *client_security_err_cb;
 */
/*
 oxl_client_logon_cb *client_logon_cb;
 oxl_client_goto_room_cb *client_goto_room_cb;
 oxl_client_say_cb *client_say_cb;
 */
/*
 } oxl_callbacks_t;
 */

typedef struct OXLPalCallbacksStruct {
    void (*FinishOpenPalConn)(void *data);
    void (*FinishOpenPalConnnSuccess)(void *data);
    void (*FinishOpenPalConnFail)(void *data);
    void (*FinishClosePalConn)(void *data);
    void (*FinishGotoPalURL)(void *data);
    void (*FinishPalRoomChanged)(void *data);
    void (*FinishPalAuthRequest)(void *data);
    void (*FinishPalSecurityError)(void *data);
} OXLPalCallbacks;

#endif
