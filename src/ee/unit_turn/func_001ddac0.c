#include "dotr/ee/unit_turn.h"

/* MATCHING DEVICE: the x difference goes through a float temporary and a reused variable
 * (ux = (float)px; fx = unit x; fx = ux - fx): the original keeps the converted target x in
 * its own register, which a single expression (as for y) does not reproduce. */
int SzDuel_TrimUnitDir2(int a, int b, int x, int y)
{
    struct DotrStateItem *p = SzDuel_GetUnit(a, b);
    struct DotrStateItem *q = SzDuel_GetMapUnit(x, y);
    int px = func_001a22c0((float)((x - 3) * 50));
    int py = func_001a22c0((float)((y - 3) * 50));
    int r = 0;
    int d;
    int t;
    float fx, fy, ux;

    if (p == q) {
        return SzDuel_TrimUnitDir1(a, b);
    }
    if (q != 0) {
        px += DOTR_UNIT_OFFSET_X(q);
        py += DOTR_UNIT_OFFSET_Y(q);
    }
    if (p != 0) {
        ux = (float)px;
        fx = (float)DOTR_UNIT_OFFSET_X(p) + (float)((p->locCol - 3) * 50);
        fx = ux - fx;
        fy = (float)py - ((float)DOTR_UNIT_OFFSET_Y(p) + (float)((p->locRow - 3) * 50));
        d = func_001a22c0(65536.0f * (func_001a6c18(fy, fx) / 6.2831855f));
        t = p->team == 0 ? 0 : 0x8000;
        d = (t + d + 0x400) >> 11 << 11;
        d = -((d + p->unknown_30 + 0x14000) % 0x10000);
        if (d < -32767) {
            d += 0x10000;
        } else if (d > 0x8000) {
            d -= 0x10000;
        }
        if (d != 0) {
            if (d > 0) {
                if (d > 2048) {
                    d = 2048;
                }
                p->unknown_30 += d;
            } else {
                if (d < -2048) {
                    d = -2048;
                }
                p->unknown_30 += d;
            }
            p->unknown_30 = (p->unknown_30 + 0x10000) % 0x10000;
            if (p->unknown_30 > 0x8000) {
                p->unknown_30 -= 0x10000;
            }
            r = 1;
        }
    }
    return r;
}
