#include "dotr/ee/draw_state.h"

void SzDuel_DrawJoinCard(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawJoinCard_Pipe(RhBase_GetVifPacketArea()));
}
