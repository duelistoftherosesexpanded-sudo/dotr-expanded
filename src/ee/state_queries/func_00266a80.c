#include "dotr/ee/state_queries.h"

/* 1 when a present unit in slots 0..10 (side 0 before side 1 at each index) has status bits 4,
 * 3 and 5 and an effect entry 2 of kind 23 whose SearchUnitByRange list holds (a << 8 | b)
 * (Ta_GetAgreeOwnerIDon22bitFlagLst); else 0. */
int TaPerm_IsPermCurseActive(int a, int b)
{
    int i;
    int packed = (a << 8) | b;
    struct DotrStateEntry8 *e;
    struct DotrStateEntry8 *f;
    unsigned w;
    unsigned v;

    for (i = 0; i < 11; i++) {
        if (func_001df540_word(0, i)) {
            if ((e = Ta_GetUnitEffect(0, i, 2)) && SzDuel_GetUnitStatus(0, i, 4) && SzDuel_GetUnitStatus(0, i, 3) && SzDuel_GetUnitStatus(0, i, 5)) {
                w = e->unknown_words[0];
                if ((w >> 8) == 23) {
                    if (Ta_GetAgreeOwnerIDon22bitFlagLst(packed, SearchUnitByRange(0, 0, Ta_GetCurrentSide(0, i), (w >> 6) & 3, w & 0x3f, 14))) {
                        return 1;
                    }
                }
            }
        }
        if (func_001df540_word(1, i)) {
            if ((f = Ta_GetUnitEffect(1, i, 2)) && SzDuel_GetUnitStatus(1, i, 4) && SzDuel_GetUnitStatus(1, i, 3) && SzDuel_GetUnitStatus(1, i, 5)) {
                v = f->unknown_words[0];
                if ((v >> 8) == 23) {
                    if (Ta_GetAgreeOwnerIDon22bitFlagLst(packed, SearchUnitByRange(0, 0, Ta_GetCurrentSide(1, i), (v >> 6) & 3, v & 0x3f, 14))) {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}
