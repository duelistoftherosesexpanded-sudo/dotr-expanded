#include "dotr/ee/state_queries.h"

/* Sums kind-52 entry values: type 0 adds, type 1 subtracts the low half of word 1. */
int Ta_GetEnchantOfs(void)
{
    int i;
    int total;
    int count;
    total = 0;
    count = Ta_SearchUncTypeNaturalEffectUserNum(52);
    for (i = 0; i < count; i++) {
        int slot = Ta_SearchUncTypeNaturalEffectUser(52, i);
        if (slot >= 0) {
            struct DotrStateEntry8 *e = Ta_GetUnitEffect(slot >> 8, slot & 0xff, 2);
            switch ((unsigned)e->unknown_words[1] >> 16) {
            case 0:
                total += *(unsigned short *)&e->unknown_words[1];
                break;
            case 1:
                total -= *(unsigned short *)&e->unknown_words[1];
                break;
            }
        }
    }
    return total;
}
