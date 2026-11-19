#include "dotr/ee/draw_state.h"

void ExSortDispBlur(int a0, int a1, int a2, int a3, int a4)
{
    RhBase_SetVifPacketArea(ExSortDispBlur_Pipe(RhBase_GetVifPacketArea(), a0, a1, a2, a3, a4));
}
