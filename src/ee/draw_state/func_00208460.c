#include "dotr/ee/draw_state.h"

void ExFlushTexture(void)
{
    RhBase_SetVifPacketArea(RhText_SetTextureFlush(RhBase_GetVifPacketArea()));
}
