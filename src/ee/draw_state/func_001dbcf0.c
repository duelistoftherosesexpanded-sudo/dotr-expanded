#include "dotr/ee/draw_state.h"

void SzDuel_DrawCard(int a)
{
    RhBase_SetVifPacketArea(func_001dbd30_cursor(RhBase_GetVifPacketArea(), a));
}
