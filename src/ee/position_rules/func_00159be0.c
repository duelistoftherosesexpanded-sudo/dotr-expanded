#include "dotr/ee/position_rules.h"

int TaLogic018_GetSummonPoint(int side, int x, int y)
{
    unsigned short list[49];
    unsigned char count;
    int i;
    int best;
    int here;
    unsigned short *p;

    here = SzDuel_GetUnitLoc(side == 0 ? 1 : 0, 0);
    count = func_0015a120_count(here & 0xff, here >> 8, list);
    best = 14;
    if (count == 0) {
        return -1;
    }
    for (i = 0, p = list; i < count; p++, i++) {
        if (func_001df540_word(*p & 0xff, *p >> 8) == 0) {
            int d = (((*p & 0xff) > x ? (*p & 0xff) : x) - ((*p & 0xff) < x ? (*p & 0xff) : x)) + (((*p >> 8) > y ? (*p >> 8) : y) - ((*p >> 8) < y ? (*p >> 8) : y));
            if (d < best) {
                best = d;
                here = *p;
            }
        }
    }
    return here;
}
