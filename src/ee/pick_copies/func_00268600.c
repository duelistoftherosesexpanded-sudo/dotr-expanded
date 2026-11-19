#include "dotr/ee/pick_copies.h"

int Ta_GetExzodiaPartsUnit(int side, int limbId)
{
    int kind;
    int i;
    int here;
    int best;
    int result;
    int low;
    int high;
    int spot;
    int score;

    result = -1;
    best = 255;
    for (i = 0; i < 8; i++) {
        spot = Ta_GetSommonPiecePreference(side, i);
        if ((here = SzDuel_GetMapUnitID(spot & 0xff, spot >> 8)) >= 0) {
            high = here >> 8;
            low = here & 0xff;
            kind = SzDuel_GetUnitCardNo(high, low);
            if (kind == Ta_GetExzodiaPartsCardNo(limbId)) {
                score = SzDuel_GetUnitSummonOrder(high, low);
                if (score < best) {
                    best = score;
                    result = here;
                }
            }
        }
    }
    return result;
}
