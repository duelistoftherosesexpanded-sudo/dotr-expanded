#include "dotr/ee/pick_copies.h"
int TaLogic007_IsDoCerem(int value, int index)
{
    int triple[3];
    int i;
    int word;
    Ta_GetCeremSacrifice(triple, TaHand_GetCardEffect(index, 0)->unknown_4);
    if ((word = Ta_GetSacrificeUnit(triple, value)) < 0) {
        return 0;
    }
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        int r;
        int kind;
        int y;
        int x;
        int cell;
        cell = Ta_GetUnitOwnerIDFromLstCount(word, i);
        x = cell >> 8;
        y = cell & 0xff;
        r = SzDuel_GetUnitLoc(x, y);
        kind = SzDuel_GetUnitCardNo(x, y);
        if (TaUtil_GetUnitAP(x, y, r & 0xff, r >> 8) >= 3000 || kind == 298 ||
            (kind == 305 && SzDuel_GetUnitStatus(x, y, 4) == 0) || kind == 326) {
            return 0;
        }
    }
    return 1;
}
