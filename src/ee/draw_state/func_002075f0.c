#include "dotr/ee/draw_state.h"

void ExSortPrim(int a0, int a1, int a2)
{
    RhBase_SetVifPacketArea(ExSortPrim_Pipe(RhBase_GetVifPacketArea(), a0, a1, a2));
}
