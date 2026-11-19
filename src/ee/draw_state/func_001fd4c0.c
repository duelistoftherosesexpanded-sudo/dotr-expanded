#include "dotr/ee/draw_state.h"

/* base is a char pointer copy of the cursor: with a long pointer copy the compiler keeps p in s0
 * for every store, where the original keeps writing through a0. */
DotrDrawWord SzMsg_LoadClut_Pipe(DotrDrawWord cursor, int index, int slot)
{
    long *p;
    unsigned char clut[16][4];
    int i;
    int *r;
    int n;
    char *base;

    p = (long *)cursor;
    base = (char *)cursor;
    for (i = 0; i < 16; i++) {
        clut[i][0] = clut[i][1] = clut[i][2] = gSzMsg_abRGB[index][i];
        clut[i][3] = gSzMsg_abAlpha[index][i];
    }
    p[1] = 0;
    p[0] = 0;
    ((char *)p)[3] = 16;
    p += 2;
    p[0] = 0x1000000000000004L;
    p[1] = 14;
    p += 2;
    p[0] = ((long)slot << 32) | 0x1000000000000L;
    p[1] = 0x50;
    p += 2;
    p[0] = 0;
    p[1] = 0x51;
    p += 2;
    p[0] = 0x200000008L;
    p[1] = 0x52;
    p += 2;
    p[0] = 0;
    p[1] = 0x53;
    p += 2;
    p[0] = 0x0800000000008004L;
    p[1] = 0;
    p += 2;
    r = RhMem_CopyMemory4((int *)p, (int *)clut, 16);
    n = (((long *)r - (long *)base) >> 1) - 1;
    *(unsigned short *)base = n;
    *(int *)(base + 8) = 0x11000000;
    *(int *)(base + 12) = (unsigned short)n | 0x51000000;
    return (DotrDrawWord)r;
}
