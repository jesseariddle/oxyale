//
//  oxyale-common.h
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

#include "paldefs.h"

#define OXL_MAX_STR_SZ_CAP 4096
#define IP_PORT_SZ_CAP 6
#define IP4_ADDR_SZ_CAP 16
#define IP6_MAX_ADDR_SZ_CAP 46


typedef unsigned char byte;

typedef struct OXLPalHeaderStruct {
    PAL_HEADER_FIELDS
} OXLPalHeader;

typedef struct OXLWriteReqStruct {
    uv_write_t req;
    uv_buf_t buf;
} OXLWriteReq;

void OXLLog(const char *fmt, ...);
int OXLProcessEvents();
void OXLInt2Str(int n, char *str, int len);
void *OXLAlloc(size_t size);
void OXLRelease(void *mem);
void OXLAllocBuf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void OXLDumpBufWithSize(const uv_buf_t buf, size_t size);
void OXLDumpBuf(const uv_buf_t buf);
void OXLReleaseWriteReq(OXLWriteReq *req);
void OXLWriteData(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback);

/* don't export */
/* #undef PAL_HEADER_FIELDS */

#endif
