#include "dotr/ee/slot_table.h"

DotrDrawWord SzMsg_Init_Pipe(DotrDrawWord cursor, int a0, int a1, int *out)
{
    int *state = gSzMsg_ml.words;
    int start = a1;
    int i;
    int width_shift;
    int height_shift;

    SzMsg_FreeAll();
    gSzMsg_ml.words[0] = 640;
    gSzMsg_ml.words[1] = 384;
    for (width_shift = 0; (1 << width_shift) < 640; width_shift++) {
    }
    for (height_shift = 0; (1 << height_shift) < 384; height_shift++) {
    }
    state[2] = width_shift;
    state[3] = height_shift;
    state[4] = SzText_GetClutGsAddr(3);
    if (state[4] < 0) {
        a1 += SzText_InitClutManage(a1, 1);
        state[4] = SzText_GetClutGsAddr(3);
    }
    state[5] = a1;
    for (i = 0; i < 3; i++) {
        cursor = SzMsg_LoadClut_Pipe(cursor, i, state[4] + i);
    }
    if (out) {
        *out = a1 + 480 - start;
    }
    state[6] = a0;
    state[157] = 0xfffffb; /* the word at +0x274 */
    *(unsigned short *)&state[158] = StringBlob[StringOffsetTable] & 0x1fff;
    return cursor;
}
