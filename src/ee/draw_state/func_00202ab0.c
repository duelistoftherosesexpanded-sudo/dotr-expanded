#include "dotr/ee/draw_state.h"

int SzText_DoRequest(int a0, int a1)
{
    int out;
    RhBase_SetVifPacketArea(SzText_DoRequest_Pipe(RhBase_GetVifPacketArea(), a0, a1, &out));
    return out;
}
