#include "dotr/ee/owner_draws.h"

void SzDuel_DrawCardTest(void)
{
    struct DotrOwnerDrawItem item;
    struct DotrOwnerDrawCardBlock block;

    SzDuel_ClearDrawCard((struct DotrOwnerDrawBlock *)&block);
    block.flag_13 = -1;
    block.flag_14 = 0;
    block.flag_15 = 0;
    func_001dcd70_owner(&item, SzDuel_Xax2New(data_00318270));
    func_00205c40_owner((int *)&block.look);
    block.rot = data_0031825c;
    SzDuel_StartCardSession();
    block.flag_12 = -1;
    func_001da2b0_card(&block, 0, -192.0f, 0.0f);
    SzDuel_SetupCard(&item, &block);
    SzDuel_DrawCard(&block);
    block.flag_12 = 0;
    func_001da2b0_card(&block, 0, 0.0f, -96.0f);
    SzDuel_SetupCard(&item, &block);
    SzDuel_DrawCard(&block);
    block.flag_12 = -1;
    func_00205cb0_card(&block, 192.0f, 0.0f);
    SzDuel_SetupCard(SzDuel_GetCardNode((struct DotrOwnerDrawBlock *)&block, 1) ? &item : 0, &block);
    SzDuel_DrawCard(&block);
    block.flag_12 = 0;
    func_00205cb0_card(&block, 0.0f, 96.0f);
    SzDuel_SetupCard(SzDuel_GetCardNode((struct DotrOwnerDrawBlock *)&block, 1) ? &item : 0, &block);
    SzDuel_DrawCard(&block);
    if (SzPad_Check(0, 1, 0x80)) {
        block.flag_14 = -1;
        block.unknown_30 = 0.46875f;
        block.unknown_34 = 0.48913044f;
        block.unknown_38 = 1.0f;
        block.unknown_3c = 0;
        block.flag_12 = -1;
        func_00205cb0_card(&block, 0.0f, 96.0f);
        SzDuel_SetupCard(SzDuel_GetCardNode((struct DotrOwnerDrawBlock *)&block, 1) ? &item : 0, &block);
        SzDuel_DrawCard(&block);
    }
    SzDuel_FinishCardSession();
}
