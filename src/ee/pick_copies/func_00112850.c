#include "dotr/ee/pick_copies.h"

int TaLogicCom_IsDoCerem__00112850(int side, int cardNo)
{
    int sacrifices[3];
    struct DotrPickPair *cardEffect;
    int i;
    int word;
    unsigned lim;
    int y;
    int x;
    int cell;
    int r;

    cardEffect = TaHand_GetCardEffect(cardNo, 0);
    Ta_GetCeremSacrifice(sacrifices, cardEffect->unknown_4);
    lim = (unsigned)TaHand_GetCardAPDP(cardEffect->unknown_4) >> 16;
    if ((word = Ta_GetSacrificeUnit(sacrifices, side)) < 0) {
        return 0;
    }
    for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(word); i++) {
        cell = Ta_GetUnitOwnerIDFromLstCount(word, i);
        x = cell >> 8;
        y = cell & 0xff;
        r = SzDuel_GetUnitLoc(x, y);
        if ((unsigned)TaUtil_GetUnitAP(x, y, r & 0xff, r >> 8) >= lim) {
            return 0;
        }
    }
    return 1;
}
