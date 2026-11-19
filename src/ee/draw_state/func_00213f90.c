#include "dotr/ee/draw_state.h"

void RhZSort_Finish(void)
{
    RhBase_SetVifPacketArea(RhZSort_Finish_Pipe(RhBase_GetVifPacketArea()));
}
