#include "dotr/ee/cursor_packets.h"

/* Draws model `modelId` with its calculation flag cleared, dither matrix 2 and z mode 1, through
 * SzModel_DrawWithFunc_Pipe when a callback or its argument is given and SzModel_Draw_Pipe
 * otherwise, handing either the header of a local packet; then restores the flag, z mode 0,
 * dither matrix 3 and the clamp mode ExGetRegVal(0, 4). The two words after the packet are
 * copied to out0 and out1, the second with its 3-bit field set to `mode` and its 1-bit
 * field to the low bit of that. gSzModel_pfhCache is cleared first. Returns the cursor. */
DotrDrawWord SzModel_ShotTextureWithFunc_Pipe(DotrDrawWord cursor, int modelId, int mode, struct DotrModelShotTex *out0,
                                              struct DotrModelShotTex *out1, void (*callback)(void), int arg)
{
    struct DotrModelShotWork work;
    int flag;

    flag = SzModel_GetCalcFlag(modelId);
    gSzModel_pfhCache = 0;
    cursor = ExDitherMatrixEz_Pipe(cursor, 2);
    cursor = RhBase_SetZBufferMode(cursor, 1);
    SzModel_SetCalcFlag(modelId, 0);
    if (callback != 0 || arg != 0) {
        cursor = SzModel_DrawWithFunc_Pipe(cursor, modelId, 0, 0, 0, work.header, 0, callback, arg);
    } else {
        cursor = SzModel_Draw_Pipe(cursor, modelId, 0, 0, 0, work.header, 0);
    }
    SzModel_SetCalcFlag(modelId, flag);
    cursor = RhBase_SetZBufferMode(cursor, 0);
    cursor = ExDitherMatrixEz_Pipe(cursor, 3);
    cursor = ExClampMode_Pipe(cursor, ExGetRegVal(0, 4));
    work.tex1.flag = work.tex1.mode = mode;
    *out0 = work.tex0;
    *out1 = work.tex1;
    return cursor;
}
