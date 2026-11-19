#include "dotr/ee/state_records.h"

/* Returns 0 for cardId == 999. Otherwise fills a local block through
 * SzDuel_InitCardInfo and returns the address of its 8-byte element effectIndex at +8:
 * the address of an automatic object, as in the original code. */
struct DotrStatePair *TaHand_GetCardEffect(int cardId, int effectIndex)
{
    struct DotrStatePairBlock unit;
    struct DotrStatePair *effect;
    if (cardId == 999) {
        return 0;
    }
    SzDuel_InitCardInfo((struct DotrStateBlock *)&unit, cardId);
    effect = &unit.pairs[effectIndex];
    return effect;
}
