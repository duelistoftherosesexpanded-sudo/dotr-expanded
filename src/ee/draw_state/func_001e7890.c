#include "dotr/ee/draw_state.h"

void SzEvent_Draw(void)
{
    RhBase_SetVifPacketArea(SzEvent_Draw_Pipe(RhBase_GetVifPacketArea()));
}
