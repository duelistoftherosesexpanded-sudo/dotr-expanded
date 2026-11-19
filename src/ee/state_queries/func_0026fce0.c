#include "dotr/ee/state_queries.h"

int TaHand_GetCardKind(int id)
{
    struct DotrStateEntry20 *e;
    if (id == 999) {
        return 0;
    }
    e = SzDuel_GetCardConstInfo(id);
    if (e == 0) {
        return 0;
    }
    return *(unsigned char *)e;
}
