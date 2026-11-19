#include "dotr/ee/draw_state.h"
#include "dotr/ee/slot_table.h"

DotrDrawWord Taku_Bunsyou_Set(DotrDrawWord cursor, int index, int a2, int x, int y, int unused, int e)
{
    DotrDrawWord r;
    DotrDrawWord c;

    (void)unused;
    c = RhText_SetTextureFlush(SzMsg_LoadFont_Pipe(cursor, index, a2, 0, index * 22));
    SzMsg_GetInfo(index);
    r = SzMsg_DrawRatioX_Pipe(c, index, (short)x, (short)y, e, SzMsg_DrawRect_Pipe, 1.0f, 1.0f);
    SzMsg_Free(index);
    return r;
}
