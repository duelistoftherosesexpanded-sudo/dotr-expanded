#include "dotr/ee/owner_draws.h"

DotrOwnerDrawWord Taku_Camp_Card_Get(DotrOwnerDrawWord cursor, int kind, float x, float y, int value)
{
    struct DotrOwnerDrawItem item;
    struct DotrOwnerDrawBlock block;

    SzDuel_ClearDrawCard(&block);
    block.flag_13 = -1;
    block.flag_14 = -1;
    block.flag_15 = 0;
    block.flag_12 = -1;
    block.unknown_30 = 0.5f;
    block.unknown_34 = 0.5f;
    func_001dcd70_owner(&item, 999);
    item.kind = kind;
    func_00205c40_owner(&block.unknown_14[3]);
    block.unknown_10 = 0;
    block.unknown_14[0] = value;
    block.unknown_14[1] = 0;
    block.unknown_14[2] = 0;
    cursor = SzDuel_StartCardSession_Pipe(cursor);
    SzDuel_SetCardDefPrioZ(0xffffff);
    func_00205cb0_owner(&block, x, y);
    cursor = func_001dbc40_item(cursor, SzDuel_GetCardNode(&block, 1) ? &item : 0, &block);
    return func_001d9e70_owner(SzDuel_DrawCard_Pipe(cursor, &block));
}
