#include "dotr/ee/small_state.h"


int SzPad_Check(int index, int mode, int mask)
{
    struct DotrSmallEntry *e = &gSzPad_pl.unknown_200[index];
    int held;
    int i;
    int bit;

    switch (mode) {
    case 0:
        return mask & (e->unknown_00 & ~e->unknown_02);
    case 2:
        held = 0;
        for (i = 0; i < 16; i++) {
            bit = 1 << i;
            if (((unsigned short)mask & bit) && e->unknown_18[i] >= 0 &&
                e->unknown_18[i] % e->unknown_5a.unknown_bits_0_3 == 0) {
                held = (unsigned short)(held | bit);
            }
        }
        return held | SzPad_Check(index, 0, mask);
    case 1:
        return e->unknown_00 & mask;
    default:
        return 0;
    }
}
