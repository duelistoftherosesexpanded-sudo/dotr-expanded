#include "dotr/ee/draw_state.h"

void SzFade_Draw(int a0, int a1, int a2)
{
    RhBase_SetVifPacketArea(SzFade_Draw_Pipe(RhBase_GetVifPacketArea(), a0, a1, a2));
}
