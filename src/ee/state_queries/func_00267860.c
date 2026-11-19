#include "dotr/ee/state_queries.h"
#include "dotr/ee/state_grid.h"

int Ta_IsResistAllMaster(int side, int pos)
{
    int i;
    int count;
    int lo;
    int hi;
    struct DotrStateEntry8 *e;
    int slot;

    if (pos == 0) {
        return 0;
    }
    if ((count = Ta_SearchUncTypeNaturalEffectUserNum(11)) > 0) {
        for (i = 0; i < count; i++) {
            slot = Ta_SearchUncTypeNaturalEffectUser(11, i);
            hi = slot >> 8;
            lo = slot & 0xff;
            if (SzDuel_GetUnitStatus(hi, lo, 4) && SzDuel_GetUnitStatus(hi, lo, 3) && SzDuel_GetUnitStatus(hi, lo, 5)) {
                e = Ta_GetUnitEffect(hi, lo, 2);
                if (e) {
                    switch (e->unknown_words[0] & 0x3f) {
                    case 0:
                        if ((SzDuel_GetUnitKind(side, pos) & 0x1f) == 0 && (SzDuel_GetUnitKind(side, pos) >> 5) == 0) {
                            return slot;
                        }
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
