#include "dotr/ee/small_misc.h"
int Ta_IsLDAbl(int ability, int side)
{
    int mode = (side == 0) ? 1 : 0;
    if (Ta_SearchUncTypeNaturalEffectUserNumWithSide(88, mode) > 0) { /* 88: deny leader abilities */
        return 0;
    }
    return SzDuel_GetUnitLeaderAbility(side, 0, ability, 0);
}
