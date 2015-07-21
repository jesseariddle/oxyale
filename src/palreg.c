#include <oxyale.h>
#include <palreg.h>

/* Test code: 9YAT-C8MM-GJVZL */

static inline uint32_t OXLPalRegComputeLicenseCRC(const uint32_t seed)
{
    uint32_t s, crc = PAL_REG_CRC_MAGIC;
    /*
     if ( ox_env_is_big_endian() )
	s = ox_uint32_swap_endian( seed );
    else
     */
    s = seed;
    do {
        crc = ((crc << 1) | (((crc & 0x80000000) == 0) ? 0 : 1)) ^ PAL_REG_CRC_MASK[s & 0xff];
    } while (s >>= 8);
    return crc;
}

static inline uint32_t OXLPalRegComputeLicenseCounter(const uint32_t seed, const uint32_t crc)
{
    return (seed ^ PAL_REG_MAGIC_LONG) ^ crc;
}

void OXLPalRegGenerate(OXLPalReg *reg)
{
    uint32_t seed = (uint32_t)time(NULL);
    uint32_t crc = OXLPalRegComputeLicenseCRC(seed);
    reg->crc = crc;
    reg->counter = OXLPalRegComputeLicenseCounter(seed, crc);
}

int32_t OXLPalRegTrimStringLen(const char *str)
{
    size_t i, j, len = strnlen(str, PAL_REG_STR_LEN);
    uint32_t k = 0;
    for (i = len; i--; )
	for (j = PAL_REG_TO_ASCII_LUT_LEN; j--; )
            k += (str[i] == PAL_REG_UPPER_CODE_TO_ASCII_LUT[j] |
                  str[i] == PAL_REG_LOWER_CODE_TO_ASCII_LUT[j]);
    return k;
}

void OXLPalRegTrimString(const char *regString, char *trimmedRegString)
{
    size_t i, j, k = 0, z = strnlen(regString, PAL_REG_STR_LEN);
    int8_t isValid;
    for (i = 0; i < z; ++i) {
        isValid = 0;
        for (j = PAL_REG_TO_ASCII_LUT_LEN; j--; ) {
            isValid += (regString[i] == PAL_REG_UPPER_CODE_TO_ASCII_LUT[j] |
                        regString[i] == PAL_REG_LOWER_CODE_TO_ASCII_LUT[j]);
        }
        if (isValid) {
            trimmedRegString[k++] = regString[i];
        }
    } trimmedRegString[k] = '\0';
}

static inline uint32_t OXLPalRegConvertASCIIToCode(const char ch)
{
    uint32_t code = -1;
    uint32_t i;
    for (i = PAL_REG_TO_ASCII_LUT_LEN; i--; ) {
        if (PAL_REG_UPPER_CODE_TO_ASCII_LUT[i] == ch ||
            PAL_REG_LOWER_CODE_TO_ASCII_LUT[i] == ch) {
            code = i;
        }
    }
    return code;
}

void OXLPalRegStringToReg(const char *trimmedRegString, OXLPalReg *reg)
{
    size_t pos = 0, ocnt = 0, nbits = 64, i = 0;
    int16_t sn = 0, mask = 0x0080;
    int32_t si[2];
    byte *sip = ( byte * )&si;
    const char *t = trimmedRegString;
    while (nbits--) {
        if (ocnt == 0) {
            sn = OXLPalRegConvertASCIIToCode(t[i++]);
            ocnt = 5;
        }
        if (sn & 0x10) {
            sip[pos] = (byte)(sip[pos] | mask);
        }
        sn <<= 1;
        --ocnt;
        mask >>= 1;
        if (mask == 0) {
            mask = 0x80;
            sip[++pos] = 0;
        }
    }
    reg->crc = si[0];
    reg->counter = si[1];
}

void OXLPalPrintReg(const OXLPalReg *regcode)
{
    fprintf(stderr, "regcode->counter: %u\n", regcode->counter);
    fprintf(stderr, "regcode->CRC: %u\n", regcode->crc);
}
