#include "dotr/ee/small_misc.h"

/* Returns TaLogic_IsUnitWin2 with the packed pair when the pair is not -1
 * and TaLogic_IsJudgeUnitDanger accepts it, otherwise TaLogic_IsUnitWin3
 * with `warnHideLvl`. */
int TaLogic_IsUnitWin4(int unit1, int ownerId, int warnHideLvl, int col, int row, unsigned char canSeeFaceDown)
{
    if (ownerId == -1) {
        return TaLogic_IsUnitWin3(unit1, warnHideLvl, col, row);
    }
    if (TaLogic_IsJudgeUnitDanger(ownerId >> 8, ownerId & 0xff, canSeeFaceDown) == 0) {
        return TaLogic_IsUnitWin3(unit1, warnHideLvl, col, row);
    }
    return TaLogic_IsUnitWin2(unit1, ownerId, col, row);
}
