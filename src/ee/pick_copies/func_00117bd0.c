#include "dotr/ee/pick_copies.h"

int TaLogic001_IsDoCerem(int side, int cardNo)
{
    int sacrifices[3];

    Ta_GetCeremSacrifice(sacrifices, TaHand_GetCardEffect(cardNo, 0)->unknown_4);
    if (Ta_GetSacrificeUnit(sacrifices, side) < 0) {
        return 0;
    }
    return 1;
}
