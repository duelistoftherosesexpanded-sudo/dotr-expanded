#include "dotr/ee/draw_state.h"

void SzDuel_DrawBuildDeck(void)
{
    RhBase_SetVifPacketArea(SzDuel_DrawBuildDeck_Pipe(RhBase_GetVifPacketArea()));
}
