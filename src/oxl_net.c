#include <oxyale.h>

void InitPalLogonCmd(PalLogonCmd *logonCmd,
                     char *username,
                     char *wizpass,
                     int initialRoomID,
                     uint regCRC,
                     int regCounter,
                     uint PUIDCRC,
                     int PUIDCounter)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)buf->base; */
    fprintf(stderr, "--- DEBUG: InitPalLogonCmd\n");
    logonCmd->cmdID = PAL_TX_LOGON_CMD;
    logonCmd->cmdLen = 0x80;
    logonCmd->cmdRef = 0;
    logonCmd->regCRC = regCRC;
    logonCmd->regCounter = regCounter;
    logonCmd->usernameLen = (byte)strlen(username);
    strncpy(logonCmd->username, username, PAL_USERNAME_SZ_CAP);
    strncpy(logonCmd->wizpass, wizpass, PAL_WIZ_PASS_SZ_CAP);

    /* raw */
    logonCmd->flags = PAL_NET_AUXFLAGS_AUTHENTICATE | PAL_NET_AUXFLAGS_WIN32;

    logonCmd->PUIDCounter = PUIDCounter;
    logonCmd->PUIDCRC = PUIDCRC;
    logonCmd->demoElapsed = 0;
    logonCmd->totalElapsed = 0;
    logonCmd->demoLimit = 0;

     /* initial room */
    logonCmd->initialRoomID = initialRoomID;

     /* does nothing, but is logged by server */
    /* strncpy(logonCmd->reserved, "OXYALE", 6); */
    /* strncpy(logonCmd->reserved, "PC4232", 6); */
    strncpy(logonCmd->reserved, "OPNPAL", 6);

     /* ignored on server */
    logonCmd->ulUploadRequestedProtocolVersion = 0;

     /* TODO upload capabilities plox */
    logonCmd->ulUploadCapabilities = PAL_NET_ULCAPS_ASSETS_PALACE;

     /* TODO download capabilities plox */
    logonCmd->ulDownloadCapabilities =
        PAL_NET_DLCAPS_ASSETS_PALACE |
        PAL_NET_DLCAPS_FILES_PALACE |
        PAL_NET_DLCAPS_FILES_HTTPSRVR;

     /* unused */
    logonCmd->ul2DEngineCapabilities = 0;

     /* unused */
    logonCmd->ul2DGraphicsCapabilities = 0;

     /* unused */
    logonCmd->ul3DEngineCapabilities = 0;
}

void DumpPalLogonCmd(const PalLogonCmd *logonCmd)
{
    /* oxl_net_logon_t *logon = (oxl_net_logon_t *)logon_buf->base; */
    /* raw */
    fprintf(stderr, "logonCmd->cmdID = 0x%x\n", loginCmd->cmdID);
    fprintf(stderr, "logonCmd->cmdSize = %d\n", loginCmd->cmdLen);
    fprintf(stderr, "logonCmd->cmdRef = 0x%x\n", loginCmd->cmdRef);
    fprintf(stderr, "logonCmd->regCRC = 0x%x\n", loginCmd->regCRC);
    fprintf(stderr, "logonCmd->regCounter = 0x%x\n", loginCmd->regCounter);
    fprintf(stderr, "logonCmd->usernameLen = %d\n", loginCmd->usernameLen);
    fprintf(stderr, "logonCmd->username = %s\n", loginCmd->username);
    fprintf(stderr, "logonCmd->flags = 0x%x\n", loginCmd->flags);
    fprintf(stderr, "logonCmd->PUIDCounter = 0x%x\n", loginCmd->PUIDCounter);
    fprintf(stderr, "logonCmd->PUIDCRC = 0x%x\n", loginCmd->PUIDCRC);
    fprintf(stderr, "logonCmd->demoElapsed = 0x%x\n", loginCmd->demoElapsed);
    fprintf(stderr, "logonCmd->totalElapsed = 0x%x\n", loginCmd->totalElapsed);
    fprintf(stderr, "logonCmd->demoLimit = 0x%x\n", loginCmd->demoLimit);

    /* initial room */
    fprintf(stderr, "logonCmd->roomID = %d\n", loginCmd->initialRoomID);

    /* does nothing, but is logged by server */
    fprintf(stderr, "logonCmd->reserved = %s\n", loginCmd->reserved);

    /* ignored on server */
    fprintf(stderr,
            "logonCmd->ulUploadRequestedProtocolVersion = 0x%x\n",
            logonCmd->ulUploadRequestedProtocolVersion);

    /* TODO upload capabilities plox */
    fprintf(stderr, "logonCmd->ulUploadCapabilities = 0x%x\n", logonCmd->ulUploadCapabilities);

    /* TODO download capabilities plox */
    fprintf(stderr, "logonCmd->ulDownloadCapabilities = 0x%x\n", logonCmd->ulDownloadCapabilities);

    /* unused */
    fprintf(stderr, "logonCmd->ul2DEngineCapabilities = 0x%x\n", logonCmd->ul2DEngineCapabilities);

    /* unused */
    fprintf(stderr, "logonCmd->ul2DGraphicsCapabilities = 0x%x\n", logonCmd->ul2DGraphicsCapabilities);

    /* unused */
    fprintf(stderr, "logonCmd->ul3DEngineCapabilities = 0x%x\n", logonCmd->ul3DEngineCapabilities);
}

void InitPalGotoRoomCmd(PalGotoRoomCmd *gotoRoomCmd, uint userID, short roomID)
{
    gotoRoomCmd->cmdID = PAL_TX_GOTO_ROOM_CMD;
    gotoRoomCmd->cmdLen = sizeof(short);
    gotoRoomCmd->cmdRef = userID;
    gotoRoomCmd->roomID = roomID;
}

void DumpPalSayCmd(PalSayCmd *sayCmd)
{
    fprintf(stderr, "sayCmd->cmdID = 0x%x\n", sayCmd->cmdID);
    fprintf(stderr, "sayCmd->cmdLen = %d\n", sayCmd->cmdLen);
    fprintf(stderr, "sayCmd->cmdRef = 0x%x\n", sayCmd->cmdRef);
    fprintf(stderr, "sayCmd->len = %d\n", sayCmd->len);
    fprintf(stderr, "sayCmd->msg = %s\n", sayCmd->msg);
}

void InitPalSayCmd(PalSayCmd *sayCmd, PalClient *client, char *text)
{
    /* oxl_say_t *say_msg = (oxl_say_t *)&buf.base; */
    char ct[PAL_CRYPT_CIPHERTEXT_SZ_CAP];
    PalEncrypt(&client->crypt, text, ct);
    fprintf(stderr, "text: \"%s\", ct: \"%s\"\n", text, ct);
    uint z = strnlen(ct, PAL_CRYPT_CIPHERTEXT_SZ_CAP);
    sayCmd->cmdID = PAL_TX_SAY_CMD;
    sayCmd->cmdLen = z + 3;
    sayCmd->cmdRef = client->user.id;
    sayCmd->len = (ushort)z;
    memcpy(sayCmd->msg, ct, z);
}
