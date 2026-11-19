#include "dotr/ee/state_records.h"

static inline struct DotrStateRecord *lookup(int i)
{
    struct DotrStateRecord *p = data_00317490_records;
    return p + i;
}

void SzDuel_SetPlayer(int side, int playerId)
{
    struct DotrStateRecord *p = lookup(side);
    side = p->unknown_33c;
    if (side != (unsigned char)playerId) {
        playerId &= 0x7f;
        p->unknown_33c = playerId;
    }
}
