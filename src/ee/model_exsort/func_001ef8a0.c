#include "dotr/ee/model_exsort.h"

int SzModel_ExSort_Pipe(int cursor, int index, int c, int d, int e, int f, int g)
{
    struct DotrExSortVec position;
    int chain;
    int depth;
    int packet;
    int out;
    float dz;
    float z;
    int result;

    gSzModel_pfhCache = 0;
    out = ExDitherMatrixEz_Pipe(ExSetAnchor_Pipe(cursor), 2);
    if (gSzModel_nBlurMode == 0) {
        out = SzModel_DrawMotionBlur_Pipe(RhBase_SetZBufferMode(out, 0), index);
    }
    out = RhBase_SetZBufferMode(SzModel_Draw_Pipe(RhBase_SetZBufferMode(out, 1), index, c, d, e, f, g), 0);
    out = ExDitherMatrixEz_Pipe(out, 3);
    packet = ExClampMode_Pipe(out, ExGetRegVal(0, 4));
    if (SzModel_GetCenter(index, &position) != 0) {
        depth = SzModel_GetMaxOT(index);
        dz = 0.0f;
        if (depth < 0) {
            depth = 0;
        }
        if (gSzModel_nBlurMode == 2) {
            dz = SzModel_GetRadius(index) / 2.0f;
        }
        z = Ex3d_GetSampleW(&position, dz);
        chain = ExGetMyOT();
        depth = z < (float)depth ? depth : func_001a23b0(z);
        result = ExSortAnchor_Pipe(packet, cursor, chain, depth);
    } else {
        return cursor;
    }
    return result;
}
