#include "dotr/ee/state_grid.h"

/* Tests cardNumber: 772 needs SzDuel_GetUnitKind to report kind 2 with no high bits; otherwise
 * reads bit (cardNumber - 752) of the 7-byte row SzDuel_GetUnitCardNo selects in the table at 0x29d580. */
int Do_ChkEncMatch(unsigned side, int pos, int cardNumber)
{
    if (side < 2 && pos == 0) {
        return 0;
    }
    if (cardNumber == 772) {
        if ((SzDuel_GetUnitKind(side, pos) >> 5) == 0) {
            if ((SzDuel_GetUnitKind(side, pos) & 0x1f) == 2) {
                return 1;
            }
        }
        return 0;
    }
    return (EnchMatch[SzDuel_GetUnitCardNo(side, pos) * 7 + ((cardNumber - 752) >> 3)] >> ((cardNumber - 752) % 8)) & 1;
}
