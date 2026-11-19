#include "dotr/ee/pick_copies.h"

int TaLogic008_IsDoCerem(int side, int cardNo)
{
    int triple[3];

    Ta_GetCeremSacrifice(triple, TaHand_GetCardEffect(cardNo, 0)->unknown_4);
    if (Ta_GetSacrificeUnit(triple, side) < 0) {
        return 0;
    }
    return 1;
}
