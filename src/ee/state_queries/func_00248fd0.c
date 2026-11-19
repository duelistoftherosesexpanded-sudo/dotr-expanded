#include "dotr/ee/state_queries.h"

/* Returns -1 when Do_ChkEncMatch(side, pos, equipNo) is zero; otherwise
 * Ta_GetHandEnchant3(SzDuel_GetUnitCardNo(side, pos), equipNo). */
int Ta_GetHandEnchant2(int side, int pos, int equipNo)
{
    if (Do_ChkEncMatch(side, pos, equipNo) != 0) {
        return Ta_GetHandEnchant3(SzDuel_GetUnitCardNo(side, pos), equipNo);
    }
    return -1;
}
