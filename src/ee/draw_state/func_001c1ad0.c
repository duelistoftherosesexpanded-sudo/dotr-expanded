#include "dotr/ee/draw_state.h"

void SzDuel_Draw3DView(void)
{
    RhBase_SetVifPacketArea(SzDuel_Draw3DView_Pipe(RhBase_GetVifPacketArea()));
}
