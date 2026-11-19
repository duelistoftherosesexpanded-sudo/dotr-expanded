#include "dotr/ee/small_state.h"

int SzPad_ResetCounter(int index, unsigned short mask)
{
    struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[index];
    int i;
    for (i = 0; i < 16; i++) {
        if (mask & (1 << i)) {
            e->unknown_18[i] = -e->unknown_59;
        }
    }
    return 0;
}
