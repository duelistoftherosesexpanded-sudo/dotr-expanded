#include "dotr/ee/draw_state.h"

void ExDitherMatrixEz(int a)
{
    RhBase_SetVifPacketArea(ExDitherMatrixEz_Pipe(RhBase_GetVifPacketArea(), a));
}
