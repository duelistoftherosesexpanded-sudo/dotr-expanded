#include "dotr/ee/position_rules.h"

int TaLogic014_IsUnitNaturalEffectUse(int side, int cardNo)
{
    int i;
    unsigned char hits;
    int list;
    int word;

    hits = 0;
    switch (cardNo) {
    case 612:
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 16, 14)) > 3) {
            return 1;
        }
        break;
    case 551:
        if (Ta_GetUnitNumFrom22bitFlagList(SearchUnitByRange(0, 0, side, 1, 15, 14)) > 0) {
            return 0;
        }
        list = SearchUnitByRange(0, 0, side, 2, 15, 14);
        for (i = 0; i < Ta_GetUnitNumFrom22bitFlagList(list); i++) {
            word = Ta_GetUnitOwnerIDFromLstCount(list, i);
            if (TaLogic_IsJudgeUnitDanger(word >> 8, word & 0xff, 0) == 1) {
                hits++;
            }
        }
        if (hits >= 2) {
            return 1;
        }
        break;
    }
    return 0;
}
