#include "dotr/ee/draw_state.h"

void SzDuel_DrawDialog(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawDialog_Pipe(RhBase_GetVifPacketArea()));
}
