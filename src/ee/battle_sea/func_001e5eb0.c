#include "dotr/ee/battle_sea.h"

/* 0x1e5eb0. The callee and the tables (as apLocData / abLocDataMax) are declared inside the
 * function body. */
/* DOTR_WARM_BUILD: compiled right after the fixed warm-up file; see docs/DECOMPILATION.md */

static inline float toAngle_001E5EB0(float degrees)
{
    return 32768.0f * degrees / 180.0f;
}

int lSzBattle_DrawSea_Pipe(int arg0, int arg1, int arg2)
{
    extern struct DotrSeaPart *apLocData[];
    extern unsigned char abLocDataMax[];
    extern struct DotrSeaRot data_002922a0;
    extern int lSzBattle_DrawFakeMirror(int, int);
    struct DotrSeaRot rot;
    int i;
    int count;
    int base;
    struct DotrSeaPart *part;
    int idx;
    int phase;

    switch (arg2) {
    case 0:
        arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, 0, 0, 0, 0);
        arg0 = lSzBattle_DrawFakeMirror(arg0, 0xF);
        break;
    case 5:
    case 6:
    case 7:
    case 8:
        count = SzModel_GetAnimFrameMax(arg1, -1);
        base = (int)SzModel_GetAnimFrame(arg1);
        idx = arg2 - 5;
        part = apLocData[idx];
        for (i = 0; i < abLocDataMax[idx]; i++) {
            rot = data_002922a0;
            rot.x = (int)toAngle_001E5EB0(part->rx);
            rot.y = (int)toAngle_001E5EB0(part->ry);
            phase = (base + part->frame) % count;
            SzModel_SetAnimFrame(arg1, (float)phase);
            arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, (DotrSeaWord)part, (DotrSeaWord)&rot, 0, 0, 0);
            part++;
        }
        SzModel_SetAnimFrame(arg1, (float)base);
        break;
    default:
        arg0 = SzModel_DrawWithOutCalc_Pipe(arg0, arg1, 0, 0, 0, 0, 0);
        break;
    }
    return arg0;
}
