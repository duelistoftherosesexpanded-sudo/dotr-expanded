#include "dotr/ee/draw_state.h"

void SzDuel_DrawCustomDuel(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawCustomDuel_Pipe(RhBase_GetVifPacketArea()));
}
