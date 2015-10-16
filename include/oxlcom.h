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

#define OXL_MAX_STR_SZ_CAP 4096
#define IP_PORT_SZ_CAP 6
#define IP4_ADDR_SZ_CAP 16
#define IP6_MAX_ADDR_SZ_CAP 46

typedef unsigned char byte;
typedef void (*OXLCallback)(void *sender, const void *data, const int32_t status);

typedef struct OXLCallbackBatonStruct {
    const OXLCallback callback;
    void *sender;
    const void *data;
    const int32_t status;
} OXLCallbackBaton;

typedef struct OXLWriteBatonStruct {
    uv_write_t write;
    const uv_buf_t buf;
    const OXLCallbackBaton cb;
} OXLWriteBaton;

typedef struct OXLTimerBatonStruct {
    uv_timer_t timer;
    const OXLCallbackBaton cb;
} OXLTimerBaton;

typedef struct OXLP8StringStruct {
    uint8_t len;
    char *base;
} OXLPString;

void OXLLog(const char *fmt, ...);
int OXLProcessEvents(void);
void OXLInt2Str(int n, char *str, int len);
void *OXLAlloc(size_t size);
void OXLFree(void *buf);
void OXLAllocBuf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void OXLDumpBufWithSize(const uv_buf_t buf, size_t lenInBytes);
void OXLDumpBuf(const uv_buf_t buf);
void OXLDumpRawBufWithSize(const byte *buf, const size_t len);
OXLWriteBaton *OXLCreateWriteBaton(char *buf, uint lenInBytes, void *data, OXLCallback cb);
void OXLDestroyWriteBaton(OXLWriteBaton *wb);
/*
void OXLWriteBuf(uv_stream_t *dest, size_t len, uv_buf_t buf, uv_write_cb callback);
void OXLWrite(uv_stream_t *dest, OXLWriteBaton *wb, uv_write_cb callback);
*/

#endif
