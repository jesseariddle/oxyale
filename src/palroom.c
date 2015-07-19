//
//  oxl-pal-room.c
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#include <stdlib.h>

#include <palclient.h>
#include <palnet.h>
#include <palroom.h>

static void OXLPalFinishJoinRoom()
{
    
    /* TODO emit room joined event signal */
}

void OXLPalJoinRoom(OXLPalClient *client, uint32_t gotoRoomID)
{
    if (client->state != STATE_CONNECTED) {
        return;
    }
    
    OXLWriteReq *logonCmd = malloc(sizeof *logonCmd);
    fprintf(stderr, "--- DEBUG: allocated write_req\n");
    
    logonCmd->req.data = client;
    
    int32_t z = sizeof(OXLPalGotoRoomCmd);
    fprintf(stderr, "--- DEBUG: sizeof(OXL_GotoPalRoomCmd) is %d\n", z);
    logonCmd->buf = uv_buf_init(malloc(z), z);
    fprintf(stderr, "--- DEBUG: uv_buf_init(malloc(%d), sizeof(%d));\n", z, z);
    
    OXLPalInitGotoRoomCmd((OXLPalGotoRoomCmd *)logonCmd->buf.base, client->user.id, (short)gotoRoomID);
    
    client->room.selectedUser = NULL;
    
    fprintf(stderr, "--- DEBUG: we are about to write the following:\n");
    OXLDumpBuf(logonCmd->buf, z);
    
    /* oxl_dump_buf_p((uv_buf_t *)logon_wr->buf, z); */
    fprintf(stderr, "--- DEBUG: calling uv_write\n");
    uv_write((uv_write_t *)logonCmd,
             (uv_stream_t *)client->conn,
             &logonCmd->buf,
             1,
             OXLPalFinishJoinRoom);
}

void OXLPalLeaveRoom(OXLPalClient *client)
{
    /* Let all iptscrae ON LEAVE event handlers execute */
    /* TODO emit leave room event signal */
}

void OXLPalGotoRoom(OXLPalClient *client, uint gotoRoomID)
{
    if (client->state != STATE_CONNECTED || client->roomID == gotoRoomID) {
        return;
    }
    
    OXLPalLeaveRoom(client);
    OXLPalJoinRoom(client, gotoRoomID);
}
