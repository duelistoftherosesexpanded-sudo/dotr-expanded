#include "dotr/ee/small_misc.h"

int Ta_RemoveObjFromFlagLst2(int word, int side)
{
    int i;
    int count;
    int result;
    int owner;
    int here;
    int low;
    int high;
    int packed;

    count = Ta_GetUnitNumFrom22bitFlagList(word);
    result = word;
    for (i = 0; i < count; i++) {
        packed = Ta_GetUnitOwnerIDFromLstCount(word, i);
        high = packed >> 8;
        low = packed & 0xff;
        owner = func_0024de70_pair(high, low);
        here = SzDuel_GetUnitLoc(owner, 0);
        if (Ta_IsLDRangeAvility(high, low, 11) != 0 && owner != side) {
            SzDuel_OpenMsgPopup(0, -120, 80, 270);
            Ta_SetRangeVisual(here & 0xff, here >> 8, 0, Ta_IsLDAbl(9, owner), 1, 1, 11);
            result = ConvUnitIDtoFlagList_OFF(result, high, low);
        } else if (Ta_IsResistAll(high, low) != 0 && owner != side) {
            SzDuel_OpenMsgPopup(0, 0, 80, SzDuel_GetMsgName(83));
            result = ConvUnitIDtoFlagList_OFF(result, high, low);
        }
    }
    return result;
}
