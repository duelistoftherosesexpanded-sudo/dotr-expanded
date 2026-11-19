#include "dotr/ee/draw_state.h"

void SzText_Load(int a0, int a1, int a2)
{
    RhBase_SetVifPacketArea(SzText_Load_Pipe(RhBase_GetVifPacketArea(), a0, a1, a2));
}
