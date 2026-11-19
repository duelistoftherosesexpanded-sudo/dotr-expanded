#include "dotr/ee/block_table.h"

/* 0x1f8b50. The table gSzView_hView_words is reached through the pointer view gSzView_hView_blocks,
 * with the element cast to the fog view type. */
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

void *SzView_SetFog_Pipe(void *arg0, int arg1, int arg2, float arg3)
{
    struct DotrBlockFogView *obj;
    struct DotrBlockFogColor color;
    float *posA;
    float *posB;
    unsigned int idxA;
    unsigned int idxB;
    int scale;

    if ((arg1 < 0) || (arg1 > 0)) {
        return arg0;
    }
    obj = (struct DotrBlockFogView *)gSzView_hView_blocks[arg1];
    if (obj == 0) {
        return arg0;
    }
    if ((arg2 < 0) || (arg2 >= 10)) {
        return arg0;
    }
    posA = obj->posA[arg2];
    posB = obj->posB[arg2];
    idxB = arg2;
    idxA = arg2;
    if (arg2 != 9) {
        if (!(obj->maskA & (1 << idxA))) {
            posA = obj->posA[9];
            idxA = 9;
        }
        if (!(obj->maskB & (1 << arg2))) {
            posB = obj->posB[9];
            idxB = 9;
        }
        if (idxA == 9 && idxB == 9) {
            arg2 = 9;
        }
    }
    if ((obj->maskA & (1 << idxA)) || (obj->maskB & (1 << idxB))) {
        color = obj->color[arg2];
        scale = (unsigned int)(128.0f * arg3) & 0xFF;
        color.c[2] = (color.c[2] * scale) >> 7;
        color.c[1] = (color.c[1] * scale) >> 7;
        color.c[0] = (color.c[0] * scale) >> 7;
        arg0 = func_0020a340_fog(arg0, &color);
        arg0 = func_002093e0_fog(arg0, posA[0], posA[1], posB[0], posB[1]);
    }
    return arg0;
}
