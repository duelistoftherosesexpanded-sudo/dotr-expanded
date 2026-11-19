#include "dotr/ee/pick_copies.h"

int TaLogic002_IsDoCerem(int value, int index)
{
    int triple[3];
    int i;
    int word;
    int y;
    int x;
    int cell;
    int r;
    int k;

    Ta_GetCeremSacrifice(triple, TaHand_GetCardEffect(index, 0)->unknown_4);
    if ((word = Ta_GetSacrificeUnit(triple, value)) < 0) {
        return 0;
    }
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        cell = Ta_GetUnitOwnerIDFromLstCount(word, i);
        x = cell >> 8;
        y = cell & 0xff;
        k = SzDuel_GetUnitCardNo(x, y);
        if (k == 427 || k == 428 || k == 402 || k == 403) {
            return 0;
        }
        r = SzDuel_GetUnitLoc(x, y);
        if (TaUtil_GetUnitAP(x, y, r & 0xff, r >> 8) >= 3000) {
            return 0;
        }
    }
    return 1;
}
