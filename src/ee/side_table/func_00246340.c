#include "dotr/ee/small_misc.h"

int ShFace_Draw_Pipe(int cursor)
{
    int i;
    struct DotrMiscEntry52 *p;
    int base;
    struct DotrMiscEntry52 *q;
    int j;

    gSh_pbVif1[0] = cursor;
    for (i = 0, p = gSh_Face; i < 8; i++, p++) {
        if (p->unknown_00) {
            p->unknown_30 = SzText_LoadRequest(7, p->unknown_04[0]);
        }
    }
    base = RhBase_GetUserGsAddress();
    base += SzText_InitClutManage(base, 2);
    gSh_pbVif1[0] = func_00202b00_word(gSh_pbVif1[0], 7, base, 0);
    gSh_pbVif1[0] = func_0020f290_word(gSh_pbVif1[0]);
    SzText_FreeRequest(7, -1);
    for (q = gSh_Face, j = 0; j < 8; j++, q++) {
        if (q->unknown_00) {
            if (q->unknown_30) {
                ShFace_DrawOne(q);
            }
        }
    }
    return gSh_pbVif1[0];
}
