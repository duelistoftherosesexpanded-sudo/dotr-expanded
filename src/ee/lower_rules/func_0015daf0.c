#include "dotr/ee/lower_rules.h"

int TaLogic_GetNextActiveUnit__0015daf0(struct DotrRuleSet *set, int side, struct DotrRuleStep *step)
{
    unsigned short words[22];
    int i;
    int r;
    int position;
    unsigned short *p;
    int index;

    p = words;
    for (i = 0; i < 22; i++) {
        p[i] = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(side, words, 29, 1);
    r = TaLogic_GetAtkUnitNow(side, words, set);
    if (r >= 0) {
        return r;
    }
    for (i = 0; i < 22; i++) {
        p[i] = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(side, words, 30, 1);
    for (i = 0; i < TaLogic_GetUnitCtlLstLen(words); i++) {
        if ((SzDuel_GetUnitKind(p[i] >> 8, p[i] & 0xFF) >> 5) == 3 && TaLogic_IsUnitReadyMove(p[i]) == 1 && TaLogic_IsPickUp(p[i]) == 1) {
            return words[i];
        }
    }
    position = side << 8;
    index = func_00276c80_set(set, position);
    func_0015d8e0_entry(&set->entries[index]);
    if (((set->entries[index].kinds >> 8) & 0xFF) == 10) { /* 10: summon mission */
        if (SzDuel_InqHandPhase(side) == 1 && TaLogic_GetMinimumTask_Summon__0015f300(position, side, step) >= 0) {
            return position | 0x10000;
        }
        if (TaLogic_IsUnitReadyMove(position & 0xFFFF) == 1) {
            return position;
        }
    } else if (SzDuel_GetUnitStatus(side, 0, 0) == 0) {
        if (TaLogic_IsUnitReadyMove(position & 0xFFFF) == 1) {
            return position;
        }
    } else if (SzDuel_InqHandPhase(side) == 1 && TaLogic_GetMinimumTask_Summon__0015f300(position, side, step) >= 0) {
        return position | 0x10000;
    }
    for (i = 0; i < 22; i++) {
        p[i] = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(side, words, 31, 1);
    for (i = 0; i < TaLogic_GetUnitCtlLstLen(words); i++) {
        if (TaLogic_IsUnitReadyMove(p[i]) == 1 && TaLogic_IsPickUp(p[i]) == 1) {
            return words[i];
        }
    }
    for (i = 0; i < 22; i++) {
        p[i] = 0xFFFF;
    }
    TaLogic_GetUnitCtlLst(side, words, 31, 1);
    for (i = 0; i < TaLogic_GetUnitCtlLstLen(words); i++) {
        if (SzDuel_GetUnitStatus(p[i] >> 8, p[i] & 0xFF, 0) == 0 && TaLogic_IsPickUp(p[i]) == 1) {
            return words[i];
        }
    }
    return -1;
}
