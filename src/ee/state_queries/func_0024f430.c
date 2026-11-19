#include "dotr/ee/state_queries.h"

/* Ta_DelUnit (0x24f370) with SzDuel_GetTurnOwner() in place of `a` as the side
 * of the first SzData_CalcExp and SzDuel_AddGrave calls. */
int Ta_DelUnit2(int a, int b)
{
    struct DotrStateEntry8 *entry;
    SzData_CalcExp(SzDuel_GetTurnOwner(), SzDuel_GetUnitCardNo(a, b), 0, -4);
    SzDuel_AddGrave(SzDuel_GetTurnOwner(), SzDuel_GetUnitCardNo(a, b));
    SzDuel_DelUnit(a, b);
    if ((entry = Ta_GetUnitEffect(a, b, 4)) != 0 && (unsigned)entry->unknown_words[0] >> 8 == 81) {
        SzDuel_AddGrave((unsigned)entry->unknown_words[1] >> 24, entry->unknown_words[1] & 0xffff);
        SzData_CalcExp((unsigned)entry->unknown_words[1] >> 24, entry->unknown_words[1] & 0xffff, 0, -4);
    }
    return 0;
}
