#include <oxyale.h>

/*
uint8_t PalEnvIsBigEndian()
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

void DumpPalBuf(const uv_buf_t buf, size_t size)
{
    const int ndigits = 16;
    const int nseg = 8;
    int i = size, j;
    int c0 = 0, c1 = 0;
    int nlines = size / ndigits + (0 < size % ndigits);
    fprintf(stderr, "\n");
    for (j = 0; j < nlines; ++j) {
        fprintf(stderr, "%08x  ", c0);
        for (i = 0; i < ndigits; ++i) {
            if (i % nseg == 0 && 0 < i && c0 < size) {
                fprintf(stderr, " %02x ", buf.base[c0] & 0xff);
            }
            else if (size <= c0) {
                fprintf(stderr, "   ");
            }
            else {
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
            }
            else if (size <= c1) {
                fprintf(stderr, " ");
            }
            else {
                fprintf(stderr, ".");
            }
            
            ++c1;
        }
        
        fprintf(stderr, "\n");
    }
}

void DumpPalBufPtr(uv_buf_t *buf, size_t size)
{
    const int ndigits = 16;
    const int nseg = 8;
    int i = size, j;
    int c0 = 0, c1 = 0;
    int nlines = size / ndigits + (0 < size % ndigits);
    fprintf(stderr, "\n");
    for (j = 0; j < nlines; ++j) {
        fprintf(stderr, "%08x  ", c0);
        for (i = 0; i < ndigits; ++i) {
            if (i % nseg == 0 && 0 < i && c0 < size) {
                fprintf(stderr, " %02x ", buf->base[c0] & 0xff);
            }
            else if (size <= c0) {
                fprintf(stderr, "   ");
            }
            else {
                fprintf(stderr, "%02x ", buf->base[c0] & 0xff);
            }
            
            ++c0;
        }
        
        for (i = 0; i < ndigits; ++i) {
            if (i % nseg == 0 && 0 < i && c1 < size) {
                fprintf(stderr, " ");
            }

            if (' ' <= buf->base[c1] && buf->base[c1] <= '~') {
                fprintf(stderr, "%c", buf->base[c1] & 0xff);
            }
            else if (size <= c1) {
                fprintf(stderr, " ");
            }
            else {
                fprintf(stderr, ".");
            }
            
            ++c1;
        }
        
        fprintf(stderr, "\n");
    }
}

void FreePalBuf(write_req_t *req)
{
    fprintf(stderr, "--- DEBUG: FreePalBuf\n");
    free(req->buf.base);
    free(req);
}

void WritePalBuf(uv_stream_t *dest, size_t size, uv_buf_t buf, uv_write_cb callback)
{
    fprintf(stderr, "--- DEBUG: WritePalBuf\n");
    write_req_t *req = malloc(sizeof *req);
    req->buf = uv_buf_init(malloc(size), size);
    memcpy(req->buf.base, buf.base, size);
    fprintf(stderr, "--- DEBUG: uv_write...\n");
    uv_write((uv_write_t *)req, (uv_stream_t *)dest, &req->buf, 1, callback);
}
