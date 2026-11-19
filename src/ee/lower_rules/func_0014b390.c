#include "dotr/ee/lower_rules.h"

int TaLogic013_IsUnitInvader(int side, int pos)
{
    int id = SzDuel_GetUnitCardNo(side, pos);

    if (id == 273 || id == 30 || id == 11) {
        return 1;
    }
    return 0;
}
