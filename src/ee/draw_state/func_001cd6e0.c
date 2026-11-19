#include "dotr/ee/draw_state.h"

void SzDuel_DrawAllCard(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawAllCard_Pipe(RhBase_GetVifPacketArea()));
}
