#include "dotr/ee/draw_state.h"

void SzDuel_DrawTrade(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawTrade_Pipe(RhBase_GetVifPacketArea()));
}
