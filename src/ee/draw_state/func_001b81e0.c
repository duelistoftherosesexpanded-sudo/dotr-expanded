#include "dotr/ee/draw_state.h"

void SzDuel_DrawJoinFade(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawJoinFade_Pipe(RhBase_GetVifPacketArea()));
}
