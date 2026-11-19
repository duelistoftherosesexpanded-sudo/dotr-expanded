#include "dotr/ee/draw_state.h"

void SzDuel_DrawUnit(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawUnit_Pipe(RhBase_GetVifPacketArea()));
}
