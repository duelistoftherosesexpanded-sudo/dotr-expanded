#include "dotr/ee/state_queries.h"

/* 0 for id 999, a missing entry or a nonzero upper kind; otherwise 2 when
 * the entry's halfword at +4 is 0xffff, else 1. */
int TaHand_IsCardEffect(int id)
{
    struct DotrStateEntry20 *e;
    if (id == 999) {
        return 0;
    }
    e = SzDuel_GetCardConstInfo(id);
    if (e == 0) {
        return 0;
    }
    if ((((unsigned char *)e)[0] >> 5) == 0) {
        if (((unsigned short *)e)[2] == 0xffff) {
            return 2;
        }
        return 1;
    }
    return 0;
}
