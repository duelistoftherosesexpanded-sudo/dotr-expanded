#include "dotr/ee/small_misc.h"

/* The original inlines SzText_InitClutManage (0x202e00) and SzText_GetClutGsAddr (0x202f00),
 * which the Japanese build's symbols place in the same source file. The two static copies
 * below repeat those functions' bodies so that this function compiles on its own. */
static inline int lSzText_ResetClut(int base, int size)
{
    gSzText_tl[0].unknown_00 = base;
    gSzText_tl[0].unknown_02 = (size << 13) / 64 / 4;
    gSzText_tl[0].unknown_04 = 0;
    return gSzText_tl[0].unknown_02;
}

static inline int lSzText_TakeClut(int count)
{
    struct DotrMiscAccumulator *p = gSzText_tl;
    int r;
    if (gSzText_tl[0].unknown_04 + count > gSzText_tl[0].unknown_02) {
        return -1;
    }
    r = p->unknown_00 + p->unknown_04;
    p->unknown_04 += count;
    return r;
}

void *SzText_Load_Pipe(void *dst, char *tex, int gs, int *used)
{
    struct DotrMiscFormat *fmt = (struct DotrMiscFormat *)(tex + 16);
    struct DotrMiscTextPacket *p = dst;
    int gs0 = gs;
    int cbp = 0;
    int clutSize;
    int numberOfSections;
    if (used) *used = 0;
    if (tex == 0) return dst;
    if ((clutSize = SzText_GetClutSize(fmt)) > 0) {
        cbp = lSzText_TakeClut(clutSize);
        if (cbp < 0) {
            gs += lSzText_ResetClut(gs, 1);
            cbp = lSzText_TakeClut(clutSize);
        }
    }
    dst = RhMem_CopyMemory16(dst, tex + fmt->offset, fmt->size >> 4);
    fmt->tbp0 = gs;
    fmt->cbp = cbp;
    numberOfSections = fmt->count;
    if (clutSize != 0) {
        p->dbp = cbp;
        numberOfSections--;
        p++;
    }
    while (numberOfSections-- != 0) {
        p->dbp = gs;
        gs += 32;
        p++;
    }
    if (used) *used = gs - gs0;
    return dst;
}
