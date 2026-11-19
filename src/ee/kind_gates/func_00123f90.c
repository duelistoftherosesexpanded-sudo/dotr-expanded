#include "dotr/ee/kind_gates.h"

int TaLogic005_IsKeepHandCombo(int side, int *hand, unsigned short cardId)
{
    int kind;

    kind = TaHand_GetCardKind(cardId) >> 5;
    if (Ta_GetUnitNumFrom22bitFlagList((unsigned char)TaLogic005_Is2Head_ThunderDragon(side, hand)) == 3) {
        return 0;
    }
    /* kind is the card type (kind >> 5), so the monster types 18 (rock) and 15 (thunder)
     * can never match here. */
    switch (kind) {
    case 18:
    case 15:
        return 1;
    case 0:
        return 1;
    default:
        return 0;
    }
}
