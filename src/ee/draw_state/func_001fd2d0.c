#include "dotr/ee/draw_state.h"

int SzMsg_Init(int a0, int a1)
{
    int out;
    RhBase_SetVifPacketArea(SzMsg_Init_Pipe(RhBase_GetVifPacketArea(), a0, a1, &out));
    return out;
}
