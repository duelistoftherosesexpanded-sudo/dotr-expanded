#include "dotr/ee/state_grid.h"

/* Turns the unit at (a, b) back to its rest direction (0, or 0x8000 when
 * bit 1 of +0x57 is set) by at most 2048 per call; returns 1 when there is
 * a unit, else 0. mwccps2 compiles both `% 0x10000` as `x & 0xffff` plus a
 * sign correction for negative dividends whose added constant is 0, so in
 * the original they act as a plain 16-bit mask (result 0..0xffff even for a
 * negative dividend), where GCC gives a negative remainder. */
int SzDuel_TrimUnitDir1(int a, int b)
{
    struct DotrStateItem *p = SzDuel_GetUnit(a, b);
    int r = 0;
    int target, d;
    if (p != 0) {
        target = p->status.unknown_bit_1 ? 0x8000 : 0;
        if (p->unknown_30 != target) {
            d = (target - p->unknown_30) % 0x10000;
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
            }
        }
        r = 1;
    }
    return r;
}
