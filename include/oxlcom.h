//
//  oxlcom.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_oxlcom_h
#define oxyale_oxlcom_h

#include <stdio.h>
#include <stdlib.h>
#include <libuv/uv.h>

#ifndef YES
#define YES 1
#endif

#ifndef NO
#define NO 0
#endif

#define OXL_MAX_STR_SZ_CAP  4096
#define OXL_IP_PORT_SZ_CAP  6
#define OXL_IP4_ADDR_SZ_CAP 16
#define OXL_IP6_ADDR_SZ_CAP 46

typedef unsigned char byte;
typedef void (*OXLCallback)(void *sender, const void *data, const int32_t status);
typedef struct OXLListItemStruct OXLListItem;
/* typedef struct OXLListStruct OXLList; */

typedef struct OXLListItemStruct {
    void *data;
    OXLListItem *next;
} OXLListItem;

typedef struct OXLListStruct {
    size_t size;
    OXLListItem *head;
} OXLList;

typedef struct OXLCallbackBatonStruct {
    OXLCallback callback;
    void *sender;
    void *data;
    int32_t status;
} OXLCallbackBaton;

typedef struct OXLWriteBatonStruct {
    uv_write_t write;
    uv_buf_t buf;
    OXLCallbackBaton ocb;
} OXLWriteBaton;

typedef struct OXLTimerBatonStruct {
    uv_timer_t timer;
    OXLCallbackBaton ocb;
    void *data;
} OXLTimerBaton;

typedef struct OXLP8StringStruct {
    uint8_t len;
    char *base;
} OXLP8String;

void OXLLogDebug(const char *fmt, ...);
void OXLLogError(const char *fmt, ...);
void OXLLogInfo(const char *fmt, ...);
int OXLProcessEvents(void);
void OXLInt2Str(int n, char *str, int len);
void *OXLAlloc(size_t size);
void OXLFree(void *buf);
void OXLAllocBuf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void OXLDumpBufWithSize(const uv_buf_t buf, size_t lenInBytes);
void OXLDumpBuf(const uv_buf_t buf);
void OXLDumpRawBufWithSize(const byte *buf, const size_t len);
/* OXLWriteBaton *OXLCreateWriteBaton(char *buf, uint lenInBytes, void *data, OXLCallback cb); */
OXLWriteBaton *OXLCreateWriteBaton(void *sender, const uv_buf_t buf, OXLCallback callback);
void OXLDestroyWriteBaton(OXLWriteBaton *wb);
OXLCallbackBaton *OXLCreateCallbackBaton(void *sender, void *data, OXLCallback callback);
void OXLDestroyCallbackBaton(OXLCallbackBaton *cb);
OXLTimerBaton *OXLCreateTimerBaton(void *sender, void *data, OXLCallback callback);
void OXLDestroyTimerBaton(OXLTimerBaton *tb);
/*
void OXLWriteBuf(uv_stream_t *dest, size_t len, uv_buf_t buf, uv_write_cb callback);
void OXLWrite(uv_stream_t *dest, OXLWriteBaton *wb, uv_write_cb callback);
*/

void OXLDestroyListItem(OXLListItem *listItem);
void OXLInitListItem(OXLListItem *listItem, void *data);
OXLListItem *OXLCreateListItem(void *data);
void OXLDestroyList(OXLList *list);
void OXLInitList(OXLList *list);
OXLList *OXLCreateList();
void OXLListRemoveAll(OXLList *list);
void OXLListInsert(OXLList *list, OXLListItem *listItem, size_t index);
void OXLListAdd(OXLList *list, OXLListItem *listItem);
void OXLListAddList(OXLList *list, OXLList *listToAdd);
OXLListItem *OXLListFirst(OXLList *list);
OXLList *OXLListRest(OXLList *list);

/* returns resolved ip address in data param of callback if status == 0 */
void OXLResolveHostname(void *sender, const char *hostname, uint16_t port, OXLCallback ocb);
/* void OXLOpenSocket(const char *hostname, const uint16_t port, const OXLCallback callback); */
void OXLCloseSocket(void *sender, uv_tcp_t *socket, OXLCallback ocb);
void OXLOpenSocket(void *sender, struct addrinfo *ai, OXLCallback ocb);

#endif
