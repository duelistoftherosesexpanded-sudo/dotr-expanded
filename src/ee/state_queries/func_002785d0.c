#include "dotr/ee/state_queries.h"

int TaLogic_IsUnitWin5(struct DotrStateItem *unit, int position, int base, int col, int row, unsigned char skip)
{
    int own;
    int alt;
    int lo;
    int hi;

    if (unit == 0) {
        return 0;
    }
    hi = position >> 8;
    if (position == -1) {
        return func_00278420_score(unit, base, col, row);
    }
    lo = position & 0xff;
    if (TaLogic_IsJudgeUnitDanger(hi, lo, skip) == 0) {
        own = func_00250a60_score(unit, col, row);
        alt = func_00250bc0_score(unit, col, row);
    } else {
        if (func_00250900_score(unit) == 1) {
            alt = 0;
            own = 0;
        } else if (unit->kind >> 5) {
            alt = 0;
            own = 0;
        } else {
            own = func_00250a60_score(unit, col, row);
            alt = func_00250bc0_score(unit, col, row);
            own += func_002787f0_score(unit, func_001df540_score(hi, lo));
            alt += func_002787f0_score(unit, func_001df540_score(hi, lo));
        }
        if (func_00250900_score(func_001df540_score(hi, lo)) == 1) {
            base = 0;
        } else if (func_001e0700_score(hi, lo) >> 5) {
            base = 0;
        } else {
            base = func_002509e0_score(hi, lo, col, row);
            base += func_002787f0_score(func_001df540_score(hi, lo), unit);
        }
    }
    if (alt < own) {
        return own >= base ? 3 : 0;
    }
    if (own <= alt) {
        return base < alt ? 2 : 0;
    }
    return 0;
}
