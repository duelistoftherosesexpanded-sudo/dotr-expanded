#include "dotr/ee/small_misc.h"

int TaLogic_IsJudgeUnitDanger(int side, int pos, int canSeeFaceDown)
{
    if (canSeeFaceDown != 0) {
        return 1;
    }
    if (SzDuel_GetUnitStatus(side, pos, 2) == 1) {
        return 1;
    }
    return 0;
}
