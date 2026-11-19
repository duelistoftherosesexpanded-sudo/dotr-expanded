#include "dotr/ee/small_misc.h"

void Ta_AllUnitFade(unsigned char *mask, unsigned char *owner, int c, int dir, int flag, int value)
{
    int i;

    (void)c;
    for (i = 0; i < 22; i++) {
        if (((*(unsigned *)(mask + 8) >> (21 - i)) & 1) == 1) {
            if (i <= 10) {
                SzEffect_SetBusyFlag2(i, flag);
            } else {
                SzEffect_SetBusyFlag2((i - 11) | 0x100, flag);
            }
        } else if (dir < 0) {
            if (i <= 10) {
                if (SzDuel_GetUnitCtlGoalM(0, i) > 0 && SzDuel_GetUnitCtlGoalC(0, i) > 0) {
                    Ta_UnitFade(0, i, dir, value, 1);
                    *(unsigned *)(owner + 0xcc) = ConvUnitIDtoFlagList(*(unsigned *)(owner + 0xcc), 0, i);
                }
            } else {
                if (SzDuel_GetUnitCtlGoalM(1, i - 11) > 0 && SzDuel_GetUnitCtlGoalC(1, i - 11) > 0) {
                    Ta_UnitFade(1, i - 11, dir, value, 1);
                    *(unsigned *)(owner + 0xcc) = ConvUnitIDtoFlagList(*(unsigned *)(owner + 0xcc), 1, i - 11);
                }
            }
        } else if (((*(unsigned *)(owner + 0xcc) >> (21 - i)) & 1) == 1) {
            if (i <= 10) {
                Ta_UnitFade(0, i, dir, value, 1);
            } else {
                Ta_UnitFade(1, i - 11, dir, value, 1);
            }
        }
    }
}
