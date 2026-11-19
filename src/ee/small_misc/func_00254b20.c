#include "dotr/ee/small_misc.h"

int Ta_GetScrificeOrder(int *sacrifices, int side)
{
    int i;
    int result;
    int j;
    int set;
    int ownerId;
    int pick;
    int matchingOwnerIds[3];

    for (i = 0, result = 0; i < 3; i++) {
        if ((set = SearchUnitByRange(0, 0, side, 1, sacrifices[i], 14)) > 0) {
            for (j = 0; j < Ta_GetUnitNumFrom22bitFlagList(result); j++) {
                ownerId = Ta_GetUnitOwnerIDFromLstCount(result, j);
                if (Ta_GetAgreeOwnerIDon22bitFlagLst(ownerId, set) != 0) {
                    set = ConvUnitIDtoFlagList_OFF(set, ownerId >> 8, ownerId & 0xff);
                }
            }
            pick = Ta_GetMinimumParamUnitFromFlagList(set);
            if (pick > 0) {
                if ((matchingOwnerIds[i] = Ta_GetMinimumOrderUnitFromFlagList(pick)) >= 0) {
                    result = ConvUnitIDtoFlagList(result, matchingOwnerIds[i] >> 8, matchingOwnerIds[i] & 0xff);
                }
            }
        }
    }
    result = Ta_GetUnitNumFrom22bitFlagList(result) != 3 ? -1 : result;
    return result;
}
