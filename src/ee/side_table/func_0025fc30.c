#include "dotr/ee/small_misc.h"

void Ta_AllUnitFade2(int a, unsigned char *owner, int c, int dir, int value, int flags)
{
    int i;

    (void)a;
    (void)c;
    for (i = 0; i < 22; i++) {
        if (dir < 0) {
            if (i <= 10) {
                if (SzDuel_GetUnitCtlGoalM(0, i) > 0) {
                    Ta_UnitFade(0, i, dir, value, flags);
                    *(unsigned *)(owner + 0xcc) = ConvUnitIDtoFlagList(*(unsigned *)(owner + 0xcc), 0, i);
                }
            } else {
                if (SzDuel_GetUnitCtlGoalM(1, i - 11) > 0) {
                    Ta_UnitFade(1, i - 11, dir, value, flags);
                    *(unsigned *)(owner + 0xcc) = ConvUnitIDtoFlagList(*(unsigned *)(owner + 0xcc), 1, i - 11);
                }
            }
        } else if (((*(unsigned *)(owner + 0xcc) >> (21 - i)) & 1) == 1) {
            if (i <= 10) {
                Ta_UnitFade(0, i, dir, value, flags);
            } else {
                Ta_UnitFade(1, i - 11, dir, value, flags);
            }
        }
    }
}
