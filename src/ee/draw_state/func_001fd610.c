#include "dotr/ee/draw_state.h"

void SzMsg_LoadFont(int a0, int a1, int a2, int a3)
{
    RhBase_SetVifPacketArea(SzMsg_LoadFont_Pipe(RhBase_GetVifPacketArea(), a0, a1, a2, a3));
}
