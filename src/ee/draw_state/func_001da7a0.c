#include "dotr/ee/draw_state.h"

void SzDuel_SetupCard(int a0, int a1)
{
    RhBase_SetVifPacketArea(SzDuel_SetupCard_Pipe(RhBase_GetVifPacketArea(), a0, a1));
}
