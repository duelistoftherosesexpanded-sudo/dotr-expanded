#include "dotr/ee/small_misc.h"

/* Failing comparisons break to the shared zero return, as in TaLogic_IsStatusMatch (0x278150). */
int TaLogic_IsKeepEnchant(struct DotrMiscRule *rule, int cardId, int fusionResult)
{
    unsigned high;
    if (rule == 0) return 1;
    if (cardId != rule->unknown_00) return 1;
    switch (rule->unknown_02) {
    case 36: /* keep when the AP is at least the rule value */
        high = (unsigned)TaHand_GetCardAPDP(fusionResult) >> 16;
        if (high < rule->unknown_04) break;
        return 1;
    case 37: /* keep when the AP is at most the rule value */
        high = (unsigned)TaHand_GetCardAPDP(fusionResult) >> 16;
        if (high > rule->unknown_04) break;
        return 1;
    }
    return 0;
}
