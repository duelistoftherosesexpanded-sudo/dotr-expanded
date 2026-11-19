#include "dotr/ee/draw_state.h"

DotrDrawWord SzDuel_SetupCard2_Pipe(DotrDrawWord cursor, int unit, struct DotrDrawRowOwner *owner)
{
    int i;

    cursor = SzDuel_SetupCard_Pipe(cursor, unit, (DotrDrawWord)owner);
    if (owner->flag) {
        for (i = 0; i < owner->count; i++) {
            owner->rows[i * 48 + 24] = owner->color[2];
            owner->rows[i * 48 + 25] = owner->color[1];
            owner->rows[i * 48 + 26] = owner->color[0];
            owner->rows[i * 48 + 27] = owner->color[3];
        }
    }
    return cursor;
}
