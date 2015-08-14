//
//  palevent.h
//  oxyale
//
//  Created by Jesse Riddle on 7/19/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palevent_h
#define oxyale_palevent_h

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

typedef struct OXLPalEventStruct {
    void (*PalEventConnectSuccess)(void *sender);
    void (*PalEventConnectFail)(void *sender);
    void (*PalEventLogonSuccess)(void *sender);
    void (*PalEventLogonFail)(void *sender);
    void (*PalEventDisconnectSuccess)(void *sender);
    void (*PalEventGotoURLSuccess)(void *sender);
    void (*PalEventRoomChangeSuccess)(void *sender);
    void (*PalEventAuthRequestSuccess)(void *sender);
    void (*PalEventSecurityFail)(void *sender);
    void (*PalEventServerInfo)(void *sender);
    void (*PalEventXTalk)(void *sender, const char *message);
    
} OXLPalEvent;

#endif
