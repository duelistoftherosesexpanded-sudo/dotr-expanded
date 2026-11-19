#include "dotr/ee/slot_seeks.h"

int Ta_GetActiveTrapBUnit(int side, int sidesToCount, unsigned trapEffect)
{
    int i;
    int best;
    int result;
    unsigned *effect;

    best = 255;
    result = best;
    if (Ta_SearchUncTypeNaturalEffectUserNum(15) > 0) { /* 15: stop trap activation */
        return -1;
    }
    if (sidesToCount & 1) { /* bit 0: own side */
        for (i = 6; i < 11; i++) {
            if ((effect = func_00250740_word(side, i, 0)) != 0 && SzDuel_GetUnitKind(side, i) >> 5 == 4 && *effect >> 8 == trapEffect && SzDuel_GetUnitSummonOrder(side, i) < best) {
                best = SzDuel_GetUnitSummonOrder(side, i);
                result = (side << 8) | i;
            }
        }
    }
    if (sidesToCount >> 1) { /* bit 1: the other side */
        for (i = 6; i < 11; i++) {
            if ((effect = func_00250740_word(side == 0 ? 1 : 0, i, 0)) != 0 && SzDuel_GetUnitKind(side == 0 ? 1 : 0, i) >> 5 == 4 && *effect >> 8 == trapEffect
                && SzDuel_GetUnitSummonOrder(side == 0 ? 1 : 0, i) < best) {
                best = SzDuel_GetUnitSummonOrder(side == 0 ? 1 : 0, i);
                result = i | ((side == 0 ? 1 : 0) << 8);
            }
        }
    }
    if (result != 255) {
        return result;
    }
    return -1;
}
