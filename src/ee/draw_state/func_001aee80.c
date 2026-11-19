#include "dotr/ee/draw_state.h"

void SzDuel_DrawStageAfter(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawStageAfter_Pipe(RhBase_GetVifPacketArea()));
}
