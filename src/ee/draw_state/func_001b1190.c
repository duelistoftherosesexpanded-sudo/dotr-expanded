#include "dotr/ee/draw_state.h"

void SzDuel_DrawWindow(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawWindow_Pipe(RhBase_GetVifPacketArea()));
}
