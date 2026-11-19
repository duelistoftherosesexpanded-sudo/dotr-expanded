#include "dotr/ee/pad_toggles.h"

#define MAX0(v) ((v) > 0 ? (v) : 0)
#define CLAMP(v) (MAX0(v) < 0x8000 ? MAX0(v) : 0x8000)

void SzDuel_ActTrade(void)
{
    struct DotrControlDrift *first = &data_00317490_drift;
    struct DotrControlDrift *second;
    int sum;
    int step;

    if (data_00317490_drift.restart) {
        first->step_c86 = 1;
    }
    sum = first->unknown_c86 + (first->step_c86 << 9);
    first->unknown_c86 = CLAMP(sum);
    if (first->unknown_c86 == 0 || first->unknown_c86 >= 0x8000) {
        first->step_c86 = -first->step_c86;
    }
    step = data_00317490_drift.step_c88 << 11;
    second = &data_00317490_drift;
    second->unknown_c88 = CLAMP(second->unknown_c88 + step);
    if (second->unknown_c88 == 0 || second->unknown_c88 >= 0x8000) {
        second->step_c88 = -second->step_c88;
    }
    if (SzGame_ActMemCardProc() < 0 && lSzDuel_ActTradeProc(0) == 0) {
        lSzDuel_ActTradeProc(1);
    }
}
