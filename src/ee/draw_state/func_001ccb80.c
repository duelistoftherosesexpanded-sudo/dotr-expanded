#include "dotr/ee/draw_state.h"

void SzDuel_DrawMapEdit(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawMapEdit_Pipe(RhBase_GetVifPacketArea()));
}
