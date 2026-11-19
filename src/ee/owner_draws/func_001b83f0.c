#include "dotr/ee/owner_draws.h"

DotrOwnerDrawWord SzDuel_DrawJoinCard_Pipe(DotrOwnerDrawWord cursor)
{
    struct DotrOwnerDrawControl *control = &data_00317490;
    struct DotrOwnerDrawBlock block;

    if (data_00317490.first == 0) {
        return cursor;
    }
    if (control->second == 0) {
        return cursor;
    }
    if (control->alpha == 0) {
        return cursor;
    }
    SzDuel_ClearDrawCard(&block);
    block.flag_14 = 0;
    block.color[3] = control->alpha;
    cursor = SzDuel_StartCardSession_Pipe(cursor);
    SzDuel_SetCardDefPrioZ(0xfffffe);
    SzDuel_GetCardNodeOnScreen(&block, 0, -150.0f, 0.0f);
    cursor = SzDuel_DrawCard_Pipe(SzDuel_SetupCard2_Pipe(cursor, control->first, &block), &block);
    SzDuel_GetCardNodeOnScreen(&block, 0, 150.0f, 0.0f);
    return SzDuel_FinishCardSession_Pipe(SzDuel_DrawCard_Pipe(SzDuel_SetupCard2_Pipe(cursor, control->second, &block), &block));
}
