#include "dotr/ee/side_table.h"
#include "dotr/ee/state_grid.h"

int Ta_IsFinishFade(void)
{
    int x;
    int y;
    int total;
    int done;
    int lo;
    int hi;
    int r;
    int same;

    total = 0;
    done = 0;
    for (x = 0; x < 7; x++) {
        for (y = 0; y < 7; y++) {
            r = SzDuel_GetMapUnitID(x, y);
            if (r >= 0) {
                lo = r & 0xff;
                hi = r >> 8;
                total++;
                r = SzDuel_GetUnitCtlRateC(hi, lo);
                if ((same = r == SzDuel_GetUnitCtlGoalC(hi, lo))) {
                    if ((SzDuel_GetUnitStatus(hi, lo, 4) != 0 || lo == 0) && lo < 6) {
                        r = SzDuel_GetUnitCtlRateM(hi, lo);
                        if (r == 0 || r == 0x80) {
                            if (SzDuel_GetUnitCtlGoalM(hi, lo) == 0x80) {
                                TaEff_RestoreStdRate(hi, lo);
                            }
                            done++;
                        }
                    } else {
                        if (SzDuel_GetUnitCtlGoalC(hi, lo) == 0x80) {
                            TaEff_RestoreStdRate(hi, lo);
                        }
                        done++;
                    }
                }
            }
        }
    }
    if (total == done) {
        return 1;
    }
    return 0;
}
