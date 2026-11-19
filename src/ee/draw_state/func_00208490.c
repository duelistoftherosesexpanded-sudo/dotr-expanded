#include "dotr/ee/draw_state.h"

void ExZBufferMode(int a)
{
    RhBase_SetVifPacketArea(RhBase_SetZBufferMode(RhBase_GetVifPacketArea(), a));
}
