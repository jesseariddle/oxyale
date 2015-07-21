//
//  oxyale-common.h
//  oxyale
//
//  Created by Jesse Riddle on 7/18/15.
//  Copyright (c) 2015 Riddle Software. All rights reserved.
//

#ifndef oxyale_palcom_h
#define oxyale_palcom_h

#include <stdio.h>
#include <stdlib.h>
#include <libuv/uv.h>

#include "paldefs.h"

typedef unsigned char byte;

typedef struct OXLPalHeaderStruct {
    PAL_HEADER_FIELDS
} OXLPalHeader;

typedef struct OXLWriteReqStruct {
    uv_write_t req;
    uv_buf_t buf;
} OXLWriteReq;

void OXLAllocBuf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void OXLFreeWriteReq(OXLWriteReq *req);
void OXLWriteData(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback);
void OXLDumpBuf(const uv_buf_t buf, size_t size);

/* don't export */
/* #undef PAL_HEADER_FIELDS */

#endif
