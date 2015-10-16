#include <oxlcom.h>

/*
uint8_t PalEnvIsBigEndian(void)
{
    union {
        uint32_t ui32;
        uint8_t ui8[4];
    } bint = {0x01020304};

    return bint.ui8[0] == 1;
}

uint32_t SwapEndianUint32(const uint32_t n)
{
    uint32_t uint32 = n;
    uint32 = (uint32 & 0xFFFF0000) >> 16 | (uint32 & 0x0000FFFF) << 16;
    uint32 = (uint32 & 0xFF000000) >> 8 | (uint32 & 0x00FF0000) << 8 |
	(uint32 & 0x0000FF00) >> 8 | (uint32 & 0x000000FF) << 8;
    return uint32;
}

void Stringify(char *str, const uint32_t ui32)
{
    uint32_t t = ui32;
    size_t i, z = 0;
    while ( t /= 10 ) ++z;
    i = z;
    str[z + 1] = '\0';
    do {
        str[i--] = ( t % 10 ) + '0';
    } while ( t /= 10 );
}
*/

int OXLProcessEvents(void)
{
    return uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

void *OXLAlloc(size_t size)
{
    return malloc(size); /* could adopt a different memory strategy here */
}

void OXLFree(void *buf)
{
    free(buf); /* could adopt a different memory strategy here */
}

void OXLLog(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "--- DEBUG: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void OXLInt2Str(int n, char *str, int len) {
    snprintf(str, len - 1, "%d", n);
}


void OXLAllocBuf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    *buf = uv_buf_init(malloc(suggested_size), (uint)suggested_size);
}

void OXLDumpBufWithSize(const uv_buf_t buf, size_t size)
{
    const int ndigits = 16;
    const int nseg = 8;
    size_t i = size, j;
    int c0 = 0, c1 = 0;
    size_t nlines = size / ndigits + (0 < size % ndigits);
    fprintf(stderr, "\n");
    for (j = 0; j < nlines; ++j) {
        fprintf(stderr, "%08x  ", c0);
        for (i = 0; i < ndigits; ++i) {
            if (i % nseg == 0 && 0 < i && c0 < size) {
                fprintf(stderr, " %02x ", buf.base[c0] & 0xff);
            } else if (size <= c0) {
                fprintf(stderr, "   ");
            } else {
                fprintf(stderr, "%02x ", buf.base[c0] & 0xff);
            }
            ++c0;
        }
        for (i = 0; i < ndigits; ++i) {
            if (i % nseg == 0 && 0 < i && c1 < size) {
                fprintf(stderr, " ");
            }
            if (' ' <= buf.base[c1] && buf.base[c1] <= '~') {
                fprintf(stderr, "%c", buf.base[c1] & 0xff);
            } else if (size <= c1) {
                fprintf(stderr, " ");
            } else {
                fprintf(stderr, ".");
            }
            ++c1;
        }
        fprintf(stderr, "\n");
    }
}

void OXLDumpBuf(const uv_buf_t buf)
{
    OXLDumpRawBufWithSize((byte *)buf.base, buf.len);
}

void OXLDumpBufPtr(const uv_buf_t *buf, size_t len)
{
    OXLDumpRawBufWithSize((byte *)buf->base, len);
}

void OXLDumpRawBufWithSize(const byte *buf, const size_t len)
{
    byte *bytebuf = (byte *)buf;
    const int ndigits = 16;
    const int nseg = 8;
    size_t i = len, j;
    int c0 = 0, c1 = 0;
    size_t nlines = len / ndigits + (0 < len % ndigits);
    fprintf(stderr, "\n");
    for (j = 0; j < nlines; ++j) {
        fprintf(stderr, "%08x  ", c0);
        for (i = 0; i < ndigits; ++i) {
            if (i % nseg == 0 && 0 < i && c0 < len) {
                fprintf(stderr, " %02x ", bytebuf[c0] & 0xff);
            } else if (len <= c0) {
                fprintf(stderr, "   ");
            } else {
                fprintf(stderr, "%02x ", bytebuf[c0] & 0xff);
            }
            ++c0;
        }
        for (i = 0; i < ndigits; ++i) {
            if (i % nseg == 0 && 0 < i && c1 < len) {
                fprintf(stderr, " ");
            }
            if (' ' <= bytebuf[c1] && bytebuf[c1] <= '~') {
                fprintf(stderr, "%c", bytebuf[c1] & 0xff);
            } else if (len <= c1) {
                fprintf(stderr, " ");
            } else {
                fprintf(stderr, ".");
            }
            ++c1;
        }
        fprintf(stderr, "\n");
    }
}

void OXLDestroyWriteBaton(OXLWriteBaton *wb)
{
    OXLLog("OXLDestroyWriteBaton");
    if (wb) {
        if (wb->buf.base) {
            OXLFree(wb->buf.base);
        }
        OXLFree(wb);
    }
}

/*
void OXLWriteBuf(uv_stream_t *dest, size_t len, uv_buf_t buf, uv_write_cb callback)
{
    OXLLog("OXLWriteBuf");
    OXLWriteBaton *wb = malloc(sizeof(*wb));
    uv_buf_t buf2 = uv_buf_init(malloc((uint32_t)buf.len), (uint32_t)buf.len);
    memcpy(&wb->buf, &buf2);
    memcpy(wb->buf.base, buf.base, buf.len);
    wb->write.data = wb;
    OXLLog("Performing write...");
    uv_write(&wb->write, (uv_stream_t *)dest, &wb->buf, 1, callback);
}

void OXLWrite(uv_stream_t *dest, OXLWriteBaton *wb, uv_write_cb callback)
{
    OXLLog("OXLWrite");
    uv_write_t write;
    write.data = wb;
    uv_write(&write, dest, &wb->buf, 1, callback);
}
*/