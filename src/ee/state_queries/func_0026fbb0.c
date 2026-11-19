#include "dotr/ee/state_queries.h"

int TaHand_GetCardAPDP(int id)
{
    struct DotrStateEntry20 *e;
    if (id == 999) {
        return 0;
    }
    e = SzDuel_GetCardConstInfo(id);
    if (e == 0) {
        return 0;
    }
    return ((((unsigned short *)e)[4] & 0x1fff) << 16) | (((unsigned short *)e)[5] & 0x1fff);
}
