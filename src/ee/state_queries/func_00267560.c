#include "dotr/ee/state_queries.h"

int Ta_IsUnitHide2(struct DotrStateItem *item)
{
    int i;
    int count;
    int lo;
    int hi;
    struct DotrStateEntry8 *e;
    int slot;
    int side;
    int own;

    if (item == 0) {
        return 0;
    }
    side = SzDuel_GetTurnOwner();
    if (item->status.unknown_bit_1 == 0) {
        own = item->team;
    } else {
        own = item->team == 0 ? 1 : 0;
    }
    if (side == own) {
        return 0;
    }
    if (item->slot == 0) {
        return 0;
    }
    if ((count = Ta_SearchUncTypeNaturalEffectUserNum(68)) > 0) {
        for (i = 0; i < count; i++) {
            slot = Ta_SearchUncTypeNaturalEffectUser(68, i);
            hi = slot >> 8;
            lo = slot & 0xff;
            if (SzDuel_GetUnitStatus(hi, lo, 4) && SzDuel_GetUnitStatus(hi, lo, 3) && SzDuel_GetUnitStatus(hi, lo, 5)) {
                e = Ta_GetUnitEffect(hi, lo, 2);
                if (e) {
                    switch (e->unknown_words[0] & 0x3f) {
                    case 29:
                        return (item->kind >> 5) == 0 ? 1 : 0;
                    case 30:
                        return (item->kind >> 5) != 0 ? 1 : 0;
                    }
                }
            }
        }
    }
    return 0;
}
