#include "dotr/ee/lower_rules.h"

int TaLogic019_GetUseThunderBolt(int side)
{
    unsigned short words[22];
    int i;
    unsigned short *q;
    unsigned short *p;
    int playersStrongestMonster;
    int yugiStrongestMonster;

    p = words;

    for (i = 0, q = p; i < 22; i++) {
        *q++ = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst2(side == 0 ? 1 : 0, words, 29, 0, 0);
    playersStrongestMonster = TaLogic_GetMaxApUnit(words);
    for (i = 0; i < 22; i++) {
        *p++ = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(side, words, 29, 1);
    yugiStrongestMonster = TaLogic_GetMaxApUnit(words);
    if (playersStrongestMonster >= 0 && yugiStrongestMonster >= 0) {
        int opponentsStrongestMonsterLocation = SzDuel_GetUnitLoc(playersStrongestMonster >> 8, playersStrongestMonster & 0xFF);

        if (TaLogic_IsUnitWin(yugiStrongestMonster, playersStrongestMonster, opponentsStrongestMonsterLocation & 0xFF, opponentsStrongestMonsterLocation >> 8) < 0) {
            return playersStrongestMonster;
        }
    }
    return -1;
}
