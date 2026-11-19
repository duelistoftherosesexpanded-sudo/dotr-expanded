#include "dotr/ee/small_misc.h"

int Ta_RemoveObjFromFlagLst(int flagList, int side)
{
    int i;
    int listLen;
    int result;
    int owner;
    int low;
    int high;
    int packed;

    listLen = Ta_GetUnitNumFrom22bitFlagList(flagList);
    result = flagList;
    for (i = 0; i < listLen; i++) {
        packed = Ta_GetUnitOwnerIDFromLstCount(flagList, i);
        high = packed >> 8;
        low = packed & 0xff;
        owner = func_0024de70_pair(high, low);
        if (Ta_IsLDRangeAvility(high, low, 11) != 0 && owner != side) {
            result = ConvUnitIDtoFlagList_OFF(result, high, low);
        } else if (Ta_IsResistAll(high, low) != 0 && owner != side) {
            result = ConvUnitIDtoFlagList_OFF(result, high, low);
        }
    }
    return result;
}
