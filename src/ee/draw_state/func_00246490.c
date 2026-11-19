#include "dotr/ee/draw_state.h"

void ShFace_Draw(void)
{
    RhBase_SetVifPacketArea(ShFace_Draw_Pipe(RhBase_GetVifPacketArea()));
}
