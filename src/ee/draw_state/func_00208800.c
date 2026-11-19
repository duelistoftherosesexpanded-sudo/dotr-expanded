#include "dotr/ee/draw_state.h"

void ExFrameBuffer2Texture(int a, int b, int c, int d)
{
    RhBase_SetVifPacketArea(ExFrameBuffer2Texture_Pipe(RhBase_GetVifPacketArea(), a, b, c, d));
}
