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

int
OXLProcessEvents(void)
{
    return uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

void *
OXLAlloc(size_t size)
{
    return malloc(size); /* could adopt a different memory strategy here */
}

void
OXLFree(void *buf)
{
    free(buf); /* could adopt a different memory strategy here */
}

void
OXLLogDebug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "--- DEBUG: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void
OXLLogError(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "--- ERROR: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void
OXLLogInfo(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "--- INFO: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}

void
OXLInt2Str(int n, char *str, int len) {
    snprintf(str, len - 1, "%d", n);
}

void
OXLChainCallback(OXLCallbackBaton *ocbb)
{
    ocbb->callback(ocbb->sender, ocbb->data, ocbb->status);
}

void
OXLAllocBuf(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    *buf = uv_buf_init(malloc(suggested_size), (uint)suggested_size);
}

void
OXLDumpBufWithSize(const uv_buf_t buf, size_t size)
{
    const int ndigits = 16;
    const int nseg = 8;
    size_t i = size, j;
    int c0 = 0, c1 = 0;
    size_t nlines = size / ndigits + (0 < size % ndigits);
    fprintf(stderr, "Dumping buf with size: %ld\n", size);
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

void
OXLDumpBuf(const uv_buf_t buf)
{
    OXLDumpRawBufWithSize((byte *)buf.base, buf.len);
}

void
OXLDumpBufPtr(const uv_buf_t *buf, size_t len)
{
    OXLDumpRawBufWithSize((byte *)buf->base, len);
}

void
OXLDumpRawBufWithSize(const byte *buf, const size_t len)
{
    byte *bytebuf = (byte *)buf;
    const int ndigits = 16;
    const int nseg = 8;
    size_t i = len, j;
    int c0 = 0, c1 = 0;
    size_t nlines = len / ndigits + (0 < len % ndigits);
    fprintf(stderr, "Dumping raw buf with len: %ld\n", len);
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

OXLWriteBaton *
OXLCreateWriteBaton(void *sender, uv_buf_t buf, OXLCallback oc)
{
    OXLWriteBaton *owb = OXLAlloc(sizeof(*owb));
    
    /* wb->buf = uv_buf_init((char *)msg, msgSize); */
    /* memcpy((uv_buf_t *)&owb->buf, &buf, sizeof(owb->buf)); */
    owb->buf.len = buf.len;
    owb->buf.base = buf.base;
    
    /* copy buf contents to heap */
    /*
    uv_buf_t data = uv_buf_init(OXLAlloc(buf.len), (uint32_t)buf.len);
    memcpy(data.base, buf.base, buf.len);
    */
    
    /* OXLCallbackBaton cbInit = { .sender = sender, .data = (void *)&buf, .callback = callback };
    memcpy((OXLCallbackBaton *)&wb->cb, &cbInit, sizeof(wb->cb)); */
    OXLCallbackBaton *ocb = &owb->ocb;
    ocb->callback = oc;
    ocb->sender = sender;
    ocb->data = &buf;
    
    /* memcpy((OXLCallback)wb->callback, callback, sizeof(wb->callback)); */
    
    return owb;
}

void
OXLDestroyWriteBaton(OXLWriteBaton *owb)
{
    OXLLogDebug("OXLDestroyWriteBaton");
    if (owb) {
        if (owb->buf.base) {
            OXLFree(owb->buf.base);
        }
        OXLFree(owb);
    }
}

OXLTimerBaton *
OXLCreateTimerBaton(void *sender, void *data, OXLCallback callback)
{
    OXLTimerBaton *otb = OXLAlloc(sizeof(*otb));
    /* OXLCallbackBaton cbInit = { .sender = sender, .data = data, .callback = callback }; */
    /* memcpy((OXLCallbackBaton *)&tb->cb, &cbInit, sizeof(tb->cb)); */
    OXLCallbackBaton *ocb = &otb->ocb;
    ocb->callback = callback;
    ocb->sender = sender;
    ocb->data = data;
    
    return otb;
}

void
OXLDestroyTimerBaton(OXLTimerBaton *otb)
{
    if (otb) {
        OXLFree(otb);
    }
}

OXLCallbackBaton *
OXLCreateCallbackBaton(void *sender, void *data, OXLCallback callback)
{
    OXLCallbackBaton *ocb = OXLAlloc(sizeof(*ocb));
    /* OXLCallbackBaton ocbInit = { .sender = sender, .data = data, .callback = callback };
    memcpy(ocb, &ocbInit, sizeof(*ocb));
     */
    ocb->sender = sender;
    ocb->data = data;
    ocb->callback = callback;
    
    return ocb;
}

void
OXLDestroyCallbackBaton(OXLCallbackBaton *ocb)
{
    if (ocb) {
        if (ocb->data) {
            OXLFree(ocb->data);
        }
        OXLFree(ocb);
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


/* getaddrinfo_cb */
void
OXLAfterResolveHostname_(uv_getaddrinfo_t *req, int32_t status, struct addrinfo *res)
{
    OXLLogDebug("OXLAfterResolveHostname status: %d", status);
    OXLCallbackBaton *ocb = req->data;
    
    if (status < 0 || 0 < status) {
        OXLLogDebug("Failed to resolve host: %s", uv_strerror(status));
    } else if (res != NULL && res->ai_addr != NULL) {
        char addr[OXL_IP4_ADDR_SZ_CAP] = { '\0' };
        uv_ip4_name((struct sockaddr_in *)res->ai_addr, addr, res->ai_addrlen);
        OXLLogDebug("Resolved to %s", addr);
    }
    
    if (NULL != ocb && NULL != ocb->callback) {
        ocb->callback(ocb->sender, res, status);
        OXLDestroyCallbackBaton(ocb);
    }
    /* uv_freeaddrinfo(res); */
}

void
OXLResolveHostname(void *sender, const char *hostname, const uint16_t port, const OXLCallback oc)
{
    OXLLogDebug("ResolveHostname");
    
    /* size_t usernameLen = strlen(username) + 1; */
    /* size_t wizpassLen = strlen(wizpass) + 1; */
    
    /* client->username = calloc(z, PAL_MAX_USERNAME_LEN); */
    /* sizeof *client->username); */
    char portString[OXL_IP_PORT_SZ_CAP];
    OXLInt2Str(port, portString, OXL_IP_PORT_SZ_CAP);
    
    struct addrinfo hints; /* = OXLAlloc(sizeof(*hints)); */
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;
    
    /* OXLLogDebug("Before alloc uvgetaddrinfo"); */
    uv_getaddrinfo_t *resolver = OXLAlloc(sizeof(*resolver));
    /* OXLCallbackBaton *ocb = malloc(sizeof(*ocb)); */
    
    /* Setup stacked callback */
    /*
    ocb->sender = NULL;
    ocb->callback = oc;
    ocb->status = 0;
    */
    
    resolver->data = OXLCreateCallbackBaton(sender, NULL, oc);
    
    /* OXLLogDebug("Before uv_getaddrinfo"); */
    
    int32_t r = uv_getaddrinfo(uv_default_loop(),
                               resolver,
                               OXLAfterResolveHostname_,
                               hostname,
                               portString,
                               &hints);
    
    /* OXLLogDebug("After uv_getaddrinfo"); */
    if (r) {
        OXLLogError("getaddrinfo call returned: %s", uv_strerror(r));
        return;
    }
}

/* uv_close_cb */
void
OXLAfterCloseSocket_(uv_handle_t *handle)
{
    OXLCallbackBaton *ocb = (OXLCallbackBaton *)handle->data;
    ocb->callback(ocb->sender, ocb->data, ocb->status);
    OXLDestroyCallbackBaton(ocb);
    OXLFree(handle);
}

void
OXLCloseSocket_(void *sender, uv_tcp_t *socket, OXLCallback oc)
{
    socket->data = oc;
    uv_close((uv_handle_t *)socket, OXLAfterCloseSocket_);
}

void
OXLCloseSocket(void *sender, uv_tcp_t *socket, OXLCallback oc)
{
    OXLCloseSocket_(sender, socket, oc);
}

/* uv_connect_cb */
void
OXLAfterOpenSocket_(uv_connect_t *connect, int32_t status)
{
    OXLLogDebug("OXLAfterOpenSocket status: %d", status);
    /* OXLCallbackBaton *ocbb = (OXLCallbackBaton *)connect->data; */
    /* OXLCallback oc = connect->data;*/
    OXLCallbackBaton *ocb = connect->data;
    
    uv_tcp_t *socket = (uv_tcp_t *)connect->handle;
    
    if (status < 0) { /* error condition */
        OXLLogDebug("Error: %d. %s\n", status, uv_strerror(status));
        ocb->status = status; /* TODO or maybe -OXLEOPENSOCKETFAILURE */
        OXLCloseSocket_(ocb->sender, socket, ocb->callback);
    } else {
        if (0 < status) {
            OXLLogDebug("Socket opened with warnings.");
        }
        /* OXLLogDebug("Connected successfully to %s", client->servername); */
        /* OXLLogDebug("Connected successfully to %s", client->hostname); */
        /* client->state = STATE_HANDSHAKING; */
        /* conn->handle->data = client; */
        
        /* Start ping timer */
        /* OXLPalClientStartTxTimer_(client); */
        
        /* client->event.PalConnectEvent(client, NULL, status); */
        
        /* uv_read_start(conn->handle, OXLAllocBuf, OXLPalClientAfterRead_); */
    }
    
    if (NULL != ocb && NULL != ocb->callback) {
        ocb->callback(ocb->sender, socket, status);
        OXLDestroyCallbackBaton(ocb);
    }
    
    /* TODO Free connect here? (unsure) */
    /* OXLFree(connect); */
}

void
OXLOpenSocketWithLoop(void *sender, uv_loop_t *uv_loop, struct addrinfo *ai, const OXLCallback oc)
{
    OXLLogDebug("OXLOpenSocket");
    
    /* OXLCallbackBaton *ocb = OXLAlloc(sizeof(*ocb));*/
    /*ocb->callback = oc;
    ocb->sender = sender; */

    
    char addr[OXL_IP4_ADDR_SZ_CAP] = { '\0' };
    
    /* uv_ip4_name((struct sockaddr_in *)ai->ai_addr, addr, ai->ai_addrlen); */
    uv_ip4_name((struct sockaddr_in *)ai->ai_addr, addr, OXL_IP4_ADDR_SZ_CAP);
    
    uv_tcp_t *socket = OXLAlloc(sizeof(*socket));
    uv_tcp_init(uv_loop, socket);
    uv_connect_t *connect = OXLAlloc(sizeof(*connect));
    connect->data = OXLCreateCallbackBaton(sender, NULL, oc);
    
    /* struct sockaddr_in *dest = (struct sockaddr_in *)ai->ai_addr; */
    /* ai-> */
    
    OXLLogDebug("Connecting to %s", addr);
    int32_t tcpstatus = uv_tcp_connect(connect, socket, (const struct sockaddr *)ai->ai_addr, OXLAfterOpenSocket_);
    
    if (0 != tcpstatus) {
        OXLLogDebug("uv_tcp_connect: %s", tcpstatus, uv_strerror(tcpstatus));
    }
    
    uv_freeaddrinfo(ai);
}

void
OXLOpenSocket(void *sender, struct addrinfo *ai, OXLCallback oc)
{
    OXLOpenSocketWithLoop(sender, uv_default_loop(), ai, oc);
}

size_t
OXLListSize(OXLList *list)
{
    int64_t count = 0;
    OXLListItem *li = list->head;
    while (li->next) {
        ++count;
        li = li->next;
    }
    list->size = count;
    return count;
}

void
OXLDestroyListItem(OXLListItem *listItem)
{
    if (listItem != NULL) {
        if (listItem->data != NULL) {
            OXLFree(listItem->data);
        }
        OXLFree(listItem);
    }
}

void
OXLInitListItem(OXLListItem *listItem, void *data)
{
    listItem->data = data;
    listItem->next = NULL;
}

OXLListItem *
OXLCreateListItem(void *data)
{
    OXLListItem *li = OXLAlloc(sizeof(*li));
    OXLInitListItem(li, data);
    return li;
}

void
OXLDestroyList(OXLList *list)
{
    OXLListItem *nli, *li = list->head;
    while (li) {
        nli = li->next;
        OXLDestroyListItem(li);
        li = nli;
    }
    OXLFree(list);
}

void
OXLInitList(OXLList *list)
{
    list->head = NULL;
    list->size = 0;
}

OXLList *
OXLCreateList()
{
    OXLList *list = OXLAlloc(sizeof(OXLList));
    OXLInitList(list);
    return list;
}

void
OXLListRemoveAll(OXLList *list)
{
    OXLListItem *nli, *li = list->head;
    while (li != NULL) {
        nli = li->next;
        OXLDestroyListItem(li);
        li = nli;
    }
    
    list->size = 0;
}

OXLListItem *
OXLListFirst(OXLList *list)
{
    return list->head;
}

OXLList *
OXLCreateTailList(OXLList *list)
{
    OXLList *tailList = OXLCreateList();
    tailList->head = list->head->next;
    tailList->size = list->size - 1;
    return tailList;
}

void
OXLListRemove(OXLList *list, size_t index)
{
    if (0 == index && list->head) {
        OXLDestroyListItem(list->head);
    } else if (0 < index && list->head) {
        int32_t c = 0;
        OXLListItem *pli = list->head;
        OXLListItem *li = list->head;
        while (c < index && li->next) {
            ++c;
            li = li->next;
        }
        OXLListItem *nli = li->next;
        OXLDestroyListItem(li);
        pli->next = nli;
    }
    
    list->size--;
}

void
OXLListInsert(OXLList *list, OXLListItem *listItem, size_t index)
{
    if (0 == index && !list->head) {
        list->head = listItem;
    } else if (0 < index && list->head) {
        int32_t c = 0;
        OXLListItem *li = list->head;
        while (c < index && li->next) {
            ++c;
            li = li->next;
        }
        OXLListItem *nli = li->next;
        li->next = listItem;
        listItem->next = nli;
    }
    
    list->size++;
}

OXLListItem *
OXLListGet(OXLList *list, size_t index)
{
    size_t c = 0;
    OXLListItem *li = list->head;
    while (c < index && li) {
        ++c;
        li = li->next;
    }
    return li;
}

void
OXLListAdd(OXLList *list, OXLListItem *listItem)
{
    if (!list->head) {
        list->head = listItem;
    } else {
        OXLListItem *li = list->head;
        while (li->next) {
            li = li->next;
        }
        OXLListItem *nli = li->next;
        li->next = listItem;
        listItem->next = nli;
    }
}

void
OXLListAddList(OXLList *list, OXLList *listToAdd)
{
    if (!list->head) {
        list->head = listToAdd->head;
    } else {
        OXLListItem *li = list->head;
        while (li->next) {
            li = li->next;
        }
        li->next = listToAdd->head;
    }
}
