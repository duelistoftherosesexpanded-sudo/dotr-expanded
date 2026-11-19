#include "dotr/ee/lower_rules.h"

int TaLogic010_GetUseThunderBolt(int side)
{
    unsigned short words[22];
    int i;
    unsigned short *q;
    unsigned short *p;
    int opponentsStrongMonsterOwnerId;
    int richardsStrongestMonster;

    p = words;

    for (i = 0, q = p; i < 22; i++) {
        *q++ = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst2(side == 0 ? 1 : 0, words, 29, 0, 0);
    opponentsStrongMonsterOwnerId = TaLogic_GetMaxApUnit(words);
    for (i = 0; i < 22; i++) {
        *p++ = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(side, words, 29, 1);
    richardsStrongestMonster = TaLogic_GetMaxApUnit(words);
    if (opponentsStrongMonsterOwnerId >= 0 && richardsStrongestMonster >= 0) {
        int playersStrongestLocation = SzDuel_GetUnitLoc(opponentsStrongMonsterOwnerId >> 8, opponentsStrongMonsterOwnerId & 0xFF);

        if (TaLogic_IsUnitWin(opponentsStrongMonsterOwnerId, richardsStrongestMonster, playersStrongestLocation & 0xFF, playersStrongestLocation >> 8) > 0) {
            return opponentsStrongMonsterOwnerId;
        }
    }
    return -1;
}
